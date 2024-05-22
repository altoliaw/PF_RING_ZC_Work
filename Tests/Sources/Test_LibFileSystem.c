#include "../Headers/Test_LibFileSystem.h"
static char functionPointer_RegularExpressionCallback(char*, char*);

void Test_LibFileSystem_FindFilesProcess(void** state) {
    LibFileSystem* system = calloc(1, sizeof(LibFileSystem));
    LibFileSystem_Construct(system);

#ifdef _CmockaWinPFRINGTest_
    char filePath[1000] = "/c/msys64/opt/pf_ring";
#else
    char filePath[1000] = "/proc/net/pf_ring";
#endif
    char* filesArray = NULL;
    char fileString[FILE_NAME_FIXED_SIZE] = {'\0'};
    int arrayLength = system->pf_getAllFilesFromDirectory(system,
                                                          filePath, fileString,
                                                          &functionPointer_RegularExpressionCallback,
                                                          "^[0-9][0-9]*-.*\\.[0-9][0-9]*");
    // assert_int_equal(arrayLength, 0);

    if (filesArray != NULL) {
        test_free(filesArray);
    }

    LibFileSystem_Destruct(system);
    free(system);
}

void Test_LibFileSystem_FindFilesProcess2(void** state) {
#define FILE_NAME_FIXED_SIZE 2048
    char zcFileNameString[FILE_NAME_FIXED_SIZE] = {'\0'};
    char FileNameString[FILE_NAME_FIXED_SIZE] = {'\0'};
    // Mocking the zcString
    int length = (int)strlen("1330968-ens2f1.421;1330969-ens2f1.425;1330970-ens2f1.429;1330971-ens2f1.423;1330973-ens2f1.433;1330976-ens2f1.436;1330981-ens2f1.422;1330986-ens2f1.435;1330987-ens2f1.432;1330992-ens2f1.434;1330993-ens2f1.427;1330996-ens2f1.431;1331001-ens2f1.428;1331004-ens2f1.430;1331008-ens2f1.424;1331013-ens2f1.426");
    char* zcString = "1330968-ens2f1.421;1330969-ens2f1.425;1330970-ens2f1.429;1330971-ens2f1.423;1330973-ens2f1.433;1330976-ens2f1.436;1330981-ens2f1.422;1330986-ens2f1.435;1330987-ens2f1.432;1330992-ens2f1.434;1330993-ens2f1.427;1330996-ens2f1.431;1331001-ens2f1.428;1331004-ens2f1.430;1331008-ens2f1.424;1331013-ens2f1.426";
    strcpy(zcFileNameString, zcString);

    LibFileSystem* system = calloc(1, sizeof(LibFileSystem));
    LibFileSystem_Construct(system);

    char result = system->pf_getTargetFileNameFromString(system, zcString, FileNameString, 0x1);
    assert_int_equal((int)result, 1);
    assert_string_equal(FileNameString, "1330968-ens2f1.421");

    result = system->pf_getTargetFileNameFromString(system, zcString, FileNameString, 0x0);
    assert_int_equal((int)result, 1);
    assert_string_equal(FileNameString, "1330969-ens2f1.425");

    LibFileSystem_Destruct(system);
    free(system);
}

void Test_LibFileSystem_FindFilesProcess3(void** state) {
#define FILE_NAME_FIXED_SIZE 2048
    char zcFileNameString[FILE_NAME_FIXED_SIZE] = {'\0'};
    char FileNameString[FILE_NAME_FIXED_SIZE] = {'\0'};
    // Mocking the zcString
    int length = (int)strlen("1330968-ens2f1.421;1330969-ens2f1.425");
    char* zcString = "1330968-ens2f1.421;1330969-ens2f1.425";
    strcpy(zcFileNameString, zcString);

    LibFileSystem* system = calloc(1, sizeof(LibFileSystem));
    LibFileSystem_Construct(system);

    char result = system->pf_getTargetFileNameFromString(system, zcString, FileNameString, 0x1);
    assert_int_equal((int)result, 1);
    assert_string_equal(FileNameString, "1330968-ens2f1.421");

    result = system->pf_getTargetFileNameFromString(system, zcString, FileNameString, 0x0);
    assert_int_equal((int)result, 0);
    assert_string_equal(FileNameString, "1330969-ens2f1.425");

    LibFileSystem_Destruct(system);
    free(system);
}

void Test_LibFileSystem_FindFilesProcess4(void** state) {
#define FILE_NAME_FIXED_SIZE 2048
    char zcFileNameString[FILE_NAME_FIXED_SIZE] = {'\0'};
    char FileNameString[FILE_NAME_FIXED_SIZE] = {'\0'};
    // Mocking the zcString
    int length = (int)strlen("1330968-ens2f1.421");
    char* zcString = "1330968-ens2f1.421";
    strcpy(zcFileNameString, zcString);

    LibFileSystem* system = calloc(1, sizeof(LibFileSystem));
    LibFileSystem_Construct(system);

    char result = system->pf_getTargetFileNameFromString(system, zcString, FileNameString, 0x1);
    assert_int_equal((int)result, 0);
    assert_string_equal(FileNameString, "1330968-ens2f1.421");

    LibFileSystem_Destruct(system);
    free(system);
}

void Test_LibFileSystem_FindFilesProcess5(void** state) {
#define FILE_NAME_FIXED_SIZE 2048
    char zcFileNameString[FILE_NAME_FIXED_SIZE] = {'\0'};
    char FileNameString[FILE_NAME_FIXED_SIZE] = {'\0'};
    // Mocking the zcString
    int length = (int)strlen("");
    char* zcString = "";
    strcpy(zcFileNameString, zcString);

    LibFileSystem* system = calloc(1, sizeof(LibFileSystem));
    LibFileSystem_Construct(system);

    char result = system->pf_getTargetFileNameFromString(system, zcString, FileNameString, 0x1);
    assert_int_equal((int)result, 0);
    assert_string_equal(FileNameString, "");

    LibFileSystem_Destruct(system);
    free(system);
}

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