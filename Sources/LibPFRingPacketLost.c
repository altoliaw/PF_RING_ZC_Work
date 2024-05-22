#include "../Headers/LibPFRingPacketLost.h"

static void execPacketLost(LibPFRingPacketLost*);
static char functionPointer_RegularExpressionCallback(char*, char*);
static void openShardMemory(int*, char**);
static void closeShardMemory(int*, char**);
static void signalInterruption(int);
static char getZCinformation(char*, int*, int*, char*, char*);
static char setZCinformation(char**, int*, int*, char*, char*);
static char getProcessFile(LibPFRingPacketLost*, FILE**, char*, int, char*, char*, int);
static char verifyZcArtificialFileExistence(LibPFRingPacketLost*, char*, char*, int, char*);
static char verifyZcOriginalFileExistence(LibPFRingPacketLost*, char*, char*, int, char*, char*);
static char writeZcArtificialFile(LibPFRingPacketLost*, char*, char*, int, char*, char*, char*, long);
static char exePfCountProgram(char*, int, char*, char*, char*);
static char getPFCaller(char*, int, char*);
static char getPFLayoutParser (char*, char*, char*, char*);
static char* getNumberNormalized(char*, char*, char*, char, char*);

// Global information
// Signal information definition
interruptedControllingInformation information = {0};

/**
 * The constructor
 *
 * @param object
 */
void LibPFRingPacketLost_Construct(LibPFRingPacketLost* object) {
    object->fileSystem = NULL;
#ifdef _CmockaTest_
    object->fileSystem = test_calloc(1, sizeof(LibFileSystem));
#else
    object->fileSystem = calloc(1, sizeof(LibFileSystem));
#endif
    LibFileSystem_Construct(object->fileSystem);

    object->pf_execPacketLost = &execPacketLost;
}

/**
 * The destruct
 *
 * @param object
 */
void LibPFRingPacketLost_Destruct(LibPFRingPacketLost* object) {
    LibFileSystem_Destruct(object->fileSystem);
#ifdef _CmockaTest_
    test_free(object->fileSystem);
#else
    free(object->fileSystem);
#endif
    object->pf_execPacketLost = NULL;
}

/**
 * The execution of obtaining packet lost
 *
 * @param object
 */
static void execPacketLost(LibPFRingPacketLost* object) {
    // Signal registering
    signal(SIGINT, signalInterruption);

    // Obtaining the files from the file system
    char filePath[200] = ZC_FILE_LOCATION;
    char filesString[FILE_NAME_FIXED_SIZE] = {'\0'};
    int zcFileAmount = object->fileSystem->pf_getAllFilesFromDirectory(
        object->fileSystem,
        filePath,
        filesString,
        &functionPointer_RegularExpressionCallback,
        "^[0-9][0-9]*-.*\\.[0-9][0-9]*$");

    // Accessing the share memory
    // Declaring the fileDescriptor and opening the share memory
    int fileDescriptor = -1;

    // The pointer mapping between the virtual and physical memory
    char* ptr = NULL;
    // Opening the shared memory
    openShardMemory(&fileDescriptor, &ptr);

    // // Writing the file string into the shared memory
    char* zcDelimiter = ZC_DELIMITER;
    // // Adding the rss number (= index + 1) for each process
    int multiProcessSize = MULTI_PROCESS_SIZE;
    // Obtaining the returned boolean value
    char flag = 0x0;
    // Setting the value into the shared memory
    flag = setZCinformation(&ptr, &multiProcessSize, &zcFileAmount, filesString, zcDelimiter);

    // For reserving the path in all child processes
    char zcString[ZC_FILE_NAME_STRING_SIZE] = {'\0'};
    memcpy(zcString, ptr, strlen(ptr));
    zcString[strlen(ptr)] = '\0';

    // For reserving the string of file names or a path for a child process
    char zcFileNameCollectionString[ZC_FILE_NAME_STRING_SIZE] = {'\0'};
    int rssIndex = -1;

    // Creating POSIX semaphore tag as a mutual lock
    sem_t* sema = NULL;  // Semaphore pointer
    sema = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (sema == SEM_FAILED) {
        fprintf(stderr, "mmap mechanism failed\n");
        exit(1);
    }

    // Multi-Processing
    pid_t processState[MULTI_PROCESS_SIZE];
    for (int i = 0; i < MULTI_PROCESS_SIZE; i++) {
        pid_t currentPid = fork();

        switch (currentPid) {
            case -1: {
                // Closing the semaphore mechanism and the shared memory in the main dispatcher
                sem_close(sema);
                // Closing the shared memory in all child processes
                closeShardMemory(&fileDescriptor, &ptr);
                fprintf(stderr, "Error processes\n");
                exit(-1);
            } break;
            case 0:  // Child process
            {        // Fetching the rss index from the shared memory
                int childRssIndex = -1;
                // When the specified folder has files, the child process has its own index
                for (; childRssIndex < 0 && !information.isBreakingMonitoredLoop;) {
                    // Critical section; obtaining the rss index from the shared memory
                    sem_wait(sema);
                    {  // Critical section; obtaining from the shared memory and revising the shared memory
                       // Obtaining the contents of the shared memory
                        memcpy(zcString, ptr, strlen(ptr));
                        zcString[strlen(ptr)] = '\0';
                        // Obtaining the individual content from the shared memory
                        flag = getZCinformation(zcString, &rssIndex, &zcFileAmount, zcFileNameCollectionString, zcDelimiter);

                        // When fetching the information failed, the child process can not access the shared memory,
                        // the child process shall keep looping until the content is available in the shared memory
                        if (flag == 0x0) {
                            continue;
                        }
                        childRssIndex = --rssIndex;
                        setZCinformation(&ptr, &rssIndex, &zcFileAmount, filesString, zcDelimiter);
                        // fprintf(stdout, "Child process: id %d\n", childRssIndex);
                    }
                    sem_post(sema);
                    // Closing the semaphore mechanism
                    sem_close(sema);
                }

                // For recording the file name of the child process
                char processFileName[FILE_NAME_FIXED_SIZE] = {'\0'};
                FILE* filePtr = NULL;                
                char tmpResult = 0x0;
                char tmpProcessFileName[FILE_NAME_FIXED_SIZE] = {'\0'};
                
                // For reserving the preinted string from the execution, pfcount
                char total[PACKET_NUMBER_ARRAY_SIZE] = {'\0'};
                char dropped[PACKET_NUMBER_ARRAY_SIZE] = {'\0'};
                char droppedRate[PACKET_NUMBER_ARRAY_SIZE] = {'\0'};
                char* networkCardName = NIC_NAME;

                // Calling packet calculation function
                do {
                    // Obtaining information form the shared memory
                    memcpy(zcString, ptr, strlen(ptr));
                    zcString[strlen(ptr)] = '\0';
                    getZCinformation(zcString, &rssIndex, &zcFileAmount, zcFileNameCollectionString, zcDelimiter);
                    // fprintf(stdout, "Child process  obtains %s\n", zcString);

                    // Verifying if the destination folder has no original files
                    if (zcFileAmount == 0) {
                        // The previous file name was reserved before executing getProcessFile(.).
                        // Thereby, the new file from original ZC file shall be removed.
                        if (strlen(processFileName) > 0) {
                            // Verifying if the artificial zc file exists
                            tmpResult = verifyZcArtificialFileExistence(object, processFileName, filePath, (int)strlen(filePath), tmpProcessFileName);
                            // When the artificial zc file exists, ...
                            if (tmpResult >= 0x1) {
                                remove(tmpProcessFileName);
                                tmpProcessFileName[0] = '\0';
                            }
                            // Deleting the file name
                            // fprintf(stdout, "Deleting child name %s\n", processFileName);
                            processFileName[0] = '\0';
                        }
                        // Obtaining the result
                        getProcessFile(object, &filePtr, zcFileNameCollectionString, childRssIndex, processFileName, filePath, (int)strlen(filePath));
                        // isReceivedFileName = getProcessFile(object, &filePtr, zcFileNameCollectionString, childRssIndex, processFileName, filePath, (int)strlen(filePath));
                    } else {  // Verifying if the destination folder has files
                        // ${processFileName} has a file name
                        if (strlen(processFileName) > 0) {
                            // Checking the file, namely ${processFileName}, is valid
                            tmpResult = verifyZcOriginalFileExistence(object, processFileName, filePath, (int)strlen(filePath), tmpProcessFileName, NULL);
                            // fprintf(stdout, "%d || %s\n", (int)tmpResult, processFileName);
                            // If ${processFileName} does not exist in the original zc files, ...
                            if (tmpResult == 0x0) {
                                // Verifying if the zc artificial file exists, ...
                                char tmpResult2 = verifyZcArtificialFileExistence(object, processFileName, filePath, (int)strlen(filePath), tmpProcessFileName);
                                // fprintf(stdout, "%d || %s\n", (int)tmpResult2, processFileName);
                                // When the artificial zc file exists, ...
                                if (tmpResult2 >= 0x1) {
                                    // fprintf(stdout, "ooooooo %s oooooooo\n", tmpProcessFileName);
                                    remove(tmpProcessFileName);
                                    tmpProcessFileName[0] = '\0';
                                }
                                // Deleting the file name
                                processFileName[0] = '\0';

                                getProcessFile(object, &filePtr, zcFileNameCollectionString, childRssIndex, processFileName, filePath, (int)strlen(filePath));
                                //  isReceivedFileName = getProcessFile(object, &filePtr, zcFileNameCollectionString, childRssIndex, processFileName, filePath, (int)strlen(filePath));
                            } else {  // ${processFileName} exists in the original zc files, ...
                                // Do nothing
                            }
                        } else {  // ${processFileName} has no file name
                            getProcessFile(object, &filePtr, zcFileNameCollectionString, childRssIndex, processFileName, filePath, (int)strlen(filePath));
                            //  isReceivedFileName = getProcessFile(object, &filePtr, zcFileNameCollectionString, childRssIndex, processFileName, filePath, (int)strlen(filePath));
                        }
                    }

                    if (zcFileAmount > 0) {
                        // Loading results from the PF-RING layout from the execution, pfcount
                        exePfCountProgram(networkCardName, childRssIndex, total, dropped, droppedRate);
                        
                        // Calculating the "out" value
                        char* ptr = NULL;
                        long outNumber = strtol(total, &ptr, 10) - strtol(dropped, &ptr, 10);

                        // Generating the output file
                        writeZcArtificialFile(object,
                                              processFileName,
                                              filePath,
                                              (int)strlen(filePath),
                                              tmpProcessFileName,
                                              total,
                                              dropped,
                                              outNumber);
                    }
                    // fprintf(stdout, "Child process%d\n", getpid());
                    sleep(2);
                } while (!information.isBreakingMonitoredLoop);
                // Closing the file pointer
                if (filePtr != NULL) {
                    fclose(filePtr);
                }
                // Closing the shared memory in all child processes
                closeShardMemory(&fileDescriptor, &ptr);
                // Closing each child process
                return;
            } break;
            default:                           // Parent process
                processState[i] = currentPid;  // Obtaining information for the main process
        }
    }

    {  // The main process (dispatcher); the one monitoring the original ZC files in the destination
        sleep(2);
        // Providing a memory for reserving the value (the value is not used latter)
        int tmpZcFileAmount = -1;
        // Providing for reserving the string of file names or a path for a child process (the value is not used latter)
        char tmpZcFileNameCollectionString[ZC_FILE_NAME_STRING_SIZE] = {'\0'};

        do {  // Discovering the number of the files in the destination folder
            zcFileAmount = object->fileSystem->pf_getAllFilesFromDirectory(
                object->fileSystem,
                filePath,
                filesString,
                &functionPointer_RegularExpressionCallback,
                "^[0-9][0-9]*-.*\\.[0-9][0-9]*$");

            // Here, the setting shall be monitored by the semaphore mechanism
            sem_wait(sema);
            {                             // Critical section
                if (zcFileAmount <= 0) {  // No specified files in the destination folder
                    filesString[0] = '\0';
                }
                // Obtaining the contents of the shared memory
                memcpy(zcString, ptr, strlen(ptr));
                zcString[strlen(ptr)] = '\0';
                // Obtaining the individual content from the shared memory
                getZCinformation(zcString, &rssIndex, &tmpZcFileAmount, tmpZcFileNameCollectionString, zcDelimiter);
                // If there is no file in the destination folder, the ptr will reserve the result as follows:
                // "0${ZC_DELIMITER}0${ZC_DELIMITER}${filesString}"
                setZCinformation(&ptr, &rssIndex, &zcFileAmount, filesString, zcDelimiter);
                // fprintf(stdout, "Main process: %s\n", ptr);
            }
            sem_post(sema);
            sleep(3);
        } while (!information.isBreakingMonitoredLoop);

        // Waiting all processes to finish the process itself
        for (int i = 0; i < MULTI_PROCESS_SIZE; i++) {
            waitpid(processState[i], NULL, 0);
        }

        // Closing the semaphore mechanism and the shared memory in the main dispatcher
        sem_close(sema);
        closeShardMemory(&fileDescriptor, &ptr);
#ifndef _CmockaTest_
        fprintf(stdout, "All Processes are interrupted\n");
#endif
    }
}

/**
 * The function pointer definition for callbacks
 *
 * @param regularRule [char*] The regular rule
 * @param verifiedString [char*] The verified string
 * @return [char] the returned value of the function pointer, 0x0 implies false; 0x1 implies true
 */
static char functionPointer_RegularExpressionCallback(char* regularRule, char* verifiedString) {
    char result = 0x0;  // default value for the boolean value, false
    // Generating the rule
    regex_t regex;
    int flag = regcomp(&regex, regularRule, 0);
    if (flag > 0) {
        fprintf(stderr, "Invalid regular expression rule generation!\n");
        exit(-1);
    }

    // Verifying the string by using the regular expression rule
    flag = regexec(&regex, verifiedString, 0, NULL, 0);
    if (!flag) {
        result = 0x1;
    } else if (flag == REG_NOMATCH) {
        result = 0x0;
    } else {
        fprintf(stderr, "Regex match failed\n");
        exit(-1);
    }
    return result;
}

/**
 * Opening the shared memory and obtaining the pointer information and the descriptor of the shared memory
 *
 * @param fileDescriptor [int*] The address of the file descriptor when opening the shared memory;
 * the default value of the argument for the parameter is -1
 * @param ptr [char**] The address of the pointer for referring to the memory map; the default value
 * of the argument (char* ptr)
 * for the parameter is NULL
 * @return [void]
 */
static void openShardMemory(int* fileDescriptor, char** ptr) {
    // Accessing the shared memory
    // Declaring the fileDescriptor and opening the share memory
    *fileDescriptor = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (*fileDescriptor == -1) {
        fprintf(stderr, "Shared memory open failed\n");
        exit(1);
    }

    // Creating a Mapping into the shared memory
    ftruncate(*fileDescriptor, SHM_SIZE);
    *ptr = (char*)mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, *fileDescriptor, 0);
    if (*ptr == MAP_FAILED) {
        fprintf(stderr, "mmap mechanism failed\n");
        exit(1);
    }
}

/**
 * Closing the pointer information and the descriptor of the shared memory
 *
 * @param fileDescriptor [int*] The address of the file descriptor when opening the shared memory;
 * the default value of the argument for the parameter is -1
 * @param ptr [char**] The address of the pointer for referring to the memory map;
 * the default value of the argument (char* ptr)
 * for the parameter is NULL
 * @return [void]
 */
static void closeShardMemory(int* fileDescriptor, char** ptr) {
    // Closing the shared memory
    if (*ptr != NULL && *ptr != MAP_FAILED) {
        // Share memory closed
        munmap(*ptr, SHM_SIZE);
        *ptr = NULL;
        if (*fileDescriptor != -1) {
            // Share memory unlinked
            shm_unlink(SHM_NAME);
            *fileDescriptor = -1;
        }
    }
}

/**
 * The signal interruption; for controlling the main process (monitor) and interrupting
 * all child processes (tasks)
 *
 * @param signo
 */
static void signalInterruption(int signo) {
    // Modifying the value to lead all processes jumping out the infinite loop
    information.isBreakingMonitoredLoop = 1;
}

/**
 * Obtaining the ZC information from the shared memory
 *
 * @param zcString
 * @param RSS
 * @param zcAmount
 * @param zcString
 * @param delimiter
 * @return [char] The boolean result in the "char" mode; 0x0 implies false and 0x1 implies true;
 * the false value shows that "zcString" has no length; the true value shows that the length is larger than or equal to 1
 */
static char getZCinformation(char* zcString, int* rssIndex, int* zcAmount, char* resultString, char* delimiter) {
    char result = 0x0;
    // Discovering the first delimiter
    char number[10] = {'\0'};

    char* ptr = strstr(zcString, delimiter);
    if (ptr != NULL) {
        // Calculating the length for the first item, for the rss index
        memcpy(number, zcString, (unsigned int)(ptr - zcString));
        number[(unsigned int)(ptr - zcString)] = '\0';
        *rssIndex = atoi(number);
        // Obtaining the second item, the number of files in the specified pf_ring folder
        char* afterDelimiterPosition = ptr + 1;
        ptr = strstr(afterDelimiterPosition, delimiter);
        if (ptr != NULL && (ptr - afterDelimiterPosition) > 0) {
            memcpy(number, afterDelimiterPosition, (unsigned int)(ptr - afterDelimiterPosition));
            number[(unsigned int)(ptr - afterDelimiterPosition)] = '\0';
            *zcAmount = atoi(number);
            // Obtaining the remained item, the number of files in the specified pf_ring folder
            afterDelimiterPosition = ptr + 1;
            memcpy(resultString, afterDelimiterPosition, strlen(zcString) - (unsigned int)(afterDelimiterPosition - zcString));
            resultString[strlen(zcString) - (unsigned int)(afterDelimiterPosition - zcString)] = '\0';
            result = 0x1;
        }
    }
    return result;
}

/**
 * Setting all values into the shared memory buffer
 *
 * @param ptr [char**]
 * @param rssIndex []
 * @param zcAmount []
 * @param resultString []
 * @param delimiter []
 * @return [char] The boolean result in the "char" mode; 0x0 implies false and 0x1 implies true;
 * the false value shows that the setting is failed; the true value shows that the setting is success
 */
static char setZCinformation(char** ptr, int* rssIndex, int* zcAmount, char* resultString, char* delimiter) {
    char result = 0x0;
    if (zcAmount <= 0) {   // No files in the specified directory
        (*ptr)[0] = '\0';  // Copying the empty string into the shared memory
    } else {
        // Writing the file string into the shared memory
        char* zcDelimiter = ZC_DELIMITER;
        // Adding the rss number (= index + 1) for each process
        sprintf(*ptr, "%d", *rssIndex);
        // Adding an category delimiter
        sprintf(*ptr + strlen(*ptr), zcDelimiter);
        // Putting the length of the value of "zcFileAmount"
        sprintf(*ptr + strlen(*ptr), "%d", *zcAmount);
        sprintf(*ptr + strlen(*ptr), zcDelimiter);
        // Putting the file names into the shared memory
        sprintf(*ptr + strlen(*ptr), resultString);
    }
    result = 0x1;
    return result;
}

/**
 * Obtaining the process's name
 *
 * @param object
 * @param filePtr
 * @param zcString [char*] The paths which is the third result recorded in the shared memory
 * @param rssIndex
 * @param processFileName
 * @param zcPath
 * @param zcPathLength
 * @return [char] The boolean result in the "char" mode; 0x0 implies false and 0x1 implies true;
 * the false value shows that "zcString" has no length; the true value shows that the length is larger than or equal to 1
 */
static char getProcessFile(LibPFRingPacketLost* object, FILE** filePtr, char* zcString,
                           int rssIndex,
                           char* processFileName,
                           char* zcPath,
                           int zcPathLength) {
    char isFoundCorrespondedFile = 0x0;

    char fileContent[FILE_NAME_FIXED_SIZE] = {'\0'};
    int fileContentLength = 0;
    // Obtaining the file name from the zcString, i controls if the next token exists; j controls
    // if the starting location is from the beginning of ${zcString}
    for (char i = 0x1, j = 0x1; i > 0x0 && strlen(zcString) > 0;) {
        i = object->fileSystem->pf_getTargetFileNameFromString(object->fileSystem, zcString, processFileName, j);
        if (j == 0x1) {
            // The next token is from the previous location.
            j = 0x0;
        }

        // Assembling the complete path and opening the file with the pointer
        zcPath[zcPathLength] = '/';
        memcpy(zcPath + zcPathLength + 1, processFileName, (int)strlen(processFileName));
        zcPath[zcPathLength + 1 + (int)strlen(processFileName)] = '\0';
        // fprintf(stdout, "%s\n", zcPath);
        *filePtr = fopen(zcPath, "rb");

        // Obtaining the length of contents in the file
        fseek(*filePtr, 0, SEEK_END);
        fileContentLength = ftell(*filePtr);
        fseek(*filePtr, 0, SEEK_SET);

        if (fileContentLength >= FILE_NAME_FIXED_SIZE - 1) {
            fprintf(stdout, "The contents are too long to reserve.\n");
            fileContentLength = FILE_NAME_FIXED_SIZE - 1;
        }

        unsigned int byteRead = fread(fileContent, sizeof(char), fileContentLength, *filePtr);
        if ((int)byteRead != fileContentLength) {
            fprintf(stdout, "fread(.) error\n");
        }
        fileContent[fileContentLength] = '\0';

        // fprintf(stdout, "%s\n", fileContent);
        // Searching for the term in ${fileContent}
        char* contentTermLocation = NULL;
        contentTermLocation = strstr(fileContent, CONTENT_lOOKING_FOR);
        if (contentTermLocation == NULL) {
            fprintf(stdout, "No CONTENT_lOOKING_FOR in the file\n");
        } else {
            // Discovering the first specified term after "CONTENT_lOOKING_FOR"
            contentTermLocation = strstr(contentTermLocation, CONTENT_lOOKING_FOR_DELIMITER);
            if (contentTermLocation == NULL) {
                fprintf(stdout, "No CONTENT_lOOKING_FOR_DELIMITER in the file\n");
            } else {
                // Moving to the first byte of the value
                contentTermLocation = contentTermLocation + 2;
                // fprintf(stdout, "%s\n", contentTermLocation);
                char* channelIDStartPosition = contentTermLocation;
                for (; contentTermLocation[0] != '\n';) {
                    contentTermLocation++;
                }
                // Copying the channel ID
                char channelIDString[10] = {'\0'};
                if (contentTermLocation - channelIDStartPosition > 10 - 1) {
                    fprintf(stdout, "The length is too large in channel id.\n");
                }
                memcpy(channelIDString, channelIDStartPosition, contentTermLocation - channelIDStartPosition);
                channelIDString[contentTermLocation - channelIDStartPosition] = '\0';
                int channelID = atoi(channelIDString);

                // Obtaining the rss index
                if (channelID == rssIndex) {
                    // fprintf(stdout, "channel id: %d\n", channelID);
                    isFoundCorrespondedFile = 0x1;
                    // Leaving the loop section
                    break;
                }
            }
        }

        // Closing the file pointer
        if (*filePtr != NULL) {
            fclose(*filePtr);
        }
    }
    // The file does not exist. The length if the process file name is 0.
    if (isFoundCorrespondedFile == 0x0) {
        processFileName[0] = '\0';
    }
    // Recovery of the zcPath
    zcPath[zcPathLength] = '\0';
    return isFoundCorrespondedFile;
}

/**
 * Checking artificial ZC files exist
 *
 * @param object
 * @param processFileName
 * @param zcPath
 * @param zcPathLength
 * @param tmpProcessFileName
 * @return [char] The value 0x0 (false) displays there exist no file; The value 0x1 (false) displays there exist the file
 */
static char verifyZcArtificialFileExistence(LibPFRingPacketLost* object,
                                            char* processFileName,
                                            char* zcPath,
                                            int zcPathLength,
                                            char* tmpProcessFileName) {
    char result = 0x0;
    char* postfixArtificialTerm = POSIX_ARTIFICIAL_TERM;
    // The variables for removing zc files
    // Adding the postfix ".zc" and assembling the folder path
    memcpy(processFileName + (int)strlen(processFileName), postfixArtificialTerm, strlen(postfixArtificialTerm));
    processFileName[(int)strlen(processFileName) + (int)strlen(postfixArtificialTerm)] = '\0';
    zcPath[zcPathLength] = '/';
    memcpy(zcPath + zcPathLength + 1, processFileName, (int)strlen(processFileName));
    zcPath[zcPathLength + 1 + (int)strlen(processFileName)] = '\0';

    // Copying the string with '\0'
    strcpy(tmpProcessFileName, zcPath);
    // Opening the file
    FILE* filePtr = fopen(zcPath, "rb");
    if (filePtr != NULL) {
        result = 0x1;
        fclose(filePtr);
    }
    // Recovering the original file path
    zcPath[zcPathLength] = '\0';
    return result;
}
/**
 * Checking original ZC files exist
 *
 * @param object
 * @param processFileName
 * @param zcPath
 * @param zcPathLength
 * @param tmpProcessFileName
 * @param content [char*] The content of the file, namely processFileName; if the value is equal to
 * NULL, the content will not be fetched
 * @return [char]
 */
static char verifyZcOriginalFileExistence(LibPFRingPacketLost* object,
                                          char* processFileName,
                                          char* zcPath,
                                          int zcPathLength,
                                          char* tmpProcessFileName,
                                          char* content) {
    char result = 0x0;
    // Verifying if the original zc file exists
    zcPath[zcPathLength] = '/';
    memcpy(zcPath + zcPathLength + 1, processFileName, (int)strlen(processFileName));
    zcPath[zcPathLength + 1 + (int)strlen(processFileName)] = '\0';

    // Opening the file
    FILE* filePtr = fopen(zcPath, "rb");
    if (content != NULL) {
        char fileContent[FILE_NAME_FIXED_SIZE] = {'\0'};
        int fileContentLength = 0;
        // Obtaining the length of contents in the file
        fseek(filePtr, 0, SEEK_END);
        fileContentLength = ftell(filePtr);
        fseek(filePtr, 0, SEEK_SET);

        if (fileContentLength >= FILE_NAME_FIXED_SIZE - 1) {
            fprintf(stdout, "The contents are too long to reserve.\n");
            fileContentLength = FILE_NAME_FIXED_SIZE - 1;
        }

        unsigned int byteRead = fread(fileContent, sizeof(char), fileContentLength, filePtr);
        if ((int)byteRead != fileContentLength) {
            fprintf(stdout, "fread(.) error\n");
        }
        fileContent[fileContentLength] = '\0';
        strcpy(content, fileContent);
    }
    if (filePtr != NULL) {
        result = 0x1;
        fclose(filePtr);
    }

    // Recovering the original file path
    zcPath[zcPathLength] = '\0';
    return result;
}

static char writeZcArtificialFile(LibPFRingPacketLost* object,
                                  char* processFileName,
                                  char* zcPath,
                                  int zcPathLength,
                                  char* tmpProcessFileName,
                                  char* packetLost,
                                  char* packetIn,
                                  long packetOut) {
    char result = 0x0;
    char* postfixArtificialTerm = POSIX_ARTIFICIAL_TERM;
    // Opening the original zc file
    char content[FILE_NAME_FIXED_SIZE] = {'\0'};
    result = verifyZcOriginalFileExistence(object, processFileName, zcPath, zcPathLength, tmpProcessFileName, content);

    // Reading the content from the opened file
    if (result >= 0x1) {
        // The variables for zc files
        // Adding the postfix ".zc" and assembling the folder path
        int tmpProcessFileNameLength = (int)strlen(processFileName);
        memcpy(processFileName + tmpProcessFileNameLength, postfixArtificialTerm, (int)strlen(postfixArtificialTerm));
        processFileName[tmpProcessFileNameLength + (int)strlen(postfixArtificialTerm)] = '\0';
        zcPath[zcPathLength] = '/';
        memcpy(zcPath + zcPathLength + 1, processFileName, (int)strlen(processFileName));
        zcPath[zcPathLength + 1 + (int)strlen(processFileName)] = '\0';

        // Recovering the process file name to the original name
        processFileName[tmpProcessFileNameLength] = '\0';
        // fprintf(stdout, "%s\n", zcPath);
        // Copying the string with '\0'
        strcpy(tmpProcessFileName, zcPath);
        // Opening the file
        FILE* filePtr = fopen(zcPath, "wb");
        if (filePtr != NULL) {
            result = 0x1;
            int contentLength = (int)strlen(content);
            char* string = "Pkt Lost               : ";
            char* string2 = "Pkt In                 : ";
            char* string3 = "Pkt Out                : ";

            sprintf(content + contentLength,
                    "%s%s\n%s%s\n%s%lu\n",
                    string,
                    packetLost,
                    string2,
                    packetIn,
                    string3,
                    packetOut);
            fwrite(content, strlen(content), 1, filePtr);
            fclose(filePtr);
        }
        // Recovering the original file path
        zcPath[zcPathLength] = '\0';
    }
    return result;
}

static char exePfCountProgram(char* networkCardName, int rssIndex, char* total, char* dropped, char* droppedRate) {
    char result = 0x0;
    FILE* filePtr = NULL;
    char output[1024];  // For obtaining contents with each line from the execution, pfcount

    // Assembling the ouputCommand, such as "sudo ./pfcount -i zc:ens2f1@1"
    char ouputCommand[PACKET_NUMBER_ARRAY_SIZE] = {'\0'};

    result = getPFCaller(networkCardName, rssIndex, ouputCommand);
    if (result == 0x0) {
        fprintf(stdout, "Assembling the command for pfcount error\n");
        return result;
    }

    // Opening the execution by using popen(.) to obtain the results printed from the execution
    filePtr = popen(ouputCommand, "r");
    if (filePtr == NULL) {
        fprintf(stdout, "Open ./pfcount error\n");
        return result;
    }

    while (fgets(output, sizeof(output), filePtr) != NULL) {
        // Determining if the string of each line can be parserd
        result = getPFLayoutParser (output, total, dropped, droppedRate);
        // Hitting the string 
        if (result == 0x1) {
            break;
        }
        // printf("%s", output);
    }

    pclose(filePtr);
    result = 0x1;
    return result;
}

static char getPFCaller(char* networkCardName, int rssIndex, char* ouputCommand) {
    char result = 0x0;
    char* command = PFRING_COMMAND;
    strcpy(ouputCommand, command);
    sprintf(ouputCommand + (int)strlen(command), "%s@%d", networkCardName, rssIndex);
    result = 0x1;
    return result;
}

/**
 * Considering if the string can be accepted from the PF-RING packet information layout;
 * the format is like "Absolute Stats: [814'567 pkts total][573'575 pkts dropped][70.4% dropped]"
 */
static char getPFLayoutParser (char* inputString, char* total, char* dropped, char* droppedRate)
{
    char result = 0x0;

    char* ptr = NULL;
    char* stringFinalAddress = (inputString + strlen(inputString));

    {// Custom function, the first term
        char* term = "Absolute Stats: [";

        ptr = strstr(inputString, term);
        if(ptr != NULL) {
            // Moving to the first number, the number implies total
            ptr = ptr + (int)strlen(term);
            ptr = getNumberNormalized(ptr, stringFinalAddress, total, ' ', &result);
        }

    }

    if (result == 0x0) {
        return result;
    }

    {// Custom function, the second term
        char* term = "[";

        ptr = strstr(ptr, term);
        if(ptr != NULL) {
            // Moving to the first number, the number implies dropped
            ptr = ptr + (int)strlen(term);
            ptr = getNumberNormalized(ptr, stringFinalAddress, dropped, ' ', &result);
        }
    }

    if (result == 0x0) {
        return result;
    }

    {// Custom function, the third term
        char* term = "[";

        ptr = strstr(ptr, term);
        if(ptr != NULL) {
            // Moving to the first number, the number implies droppedRate
            ptr = ptr + (int)strlen(term);
            ptr = getNumberNormalized(ptr, stringFinalAddress, droppedRate, '%', &result);
        }
    }

    return result;
}

/**
 * Obtaining the number part and normalizing the number
 */
static char* getNumberNormalized(char* ptr, char* stringFinalAddress, char* output, char stoppingChar, char* result) {

    // Removing the delimiter among the number and obtaining the number
    int i = 0;
    for(; ptr < stringFinalAddress && i < PACKET_NUMBER_ARRAY_SIZE - 1; ptr++, i++) {
        if (ptr[0] == stoppingChar) {
            output[i] = '\0';
            *result = 0x1;
            break;
        }
        // Skipping the term, "\'"
        if(ptr[0] == '\''){
            i--;
            continue;
        }
        // Copying the value to the destination
        output[i] = ptr[0];
    }
    output[i + 1] = '\0';
    return ptr;
}