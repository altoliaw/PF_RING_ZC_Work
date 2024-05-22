#pragma once
/** @file LibQueuingMachine
 * For obtaining number with multi-processing; this function will
 * be implemented by the semaphore mechanism in operating system because the author feels
 * like to make sure that each process has a unique number and this number will be corresponding
 * to the number of RSS
 *
 * @author Nick, Liao
 * @date 2024/04/25
 */

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
// Semaphore
#include <semaphore.h>
// Multi-processes
#include <sys/wait.h>
// The header of the regular expression
#include <regex.h>

#include "./DebugFileInclusion.h"
#include "./LibFileSystem.h"

// The defined size of the shared memory is 1024
#define SHM_SIZE 2048
// The location of the shared memory
#define SHM_NAME "/LibPFRingPacketLost"
// The location of the shared memory for the semaphore
#define SEM_NAME "/LibPFRingPacketLost_Semaphore"

// The number implies the total number of the RSS
#define MULTI_PROCESS_SIZE 4

// The ZC file locations
#define ZC_FILE_LOCATION "/proc/net/pf_ring"

// The ZC file locations
#define ZC_DELIMITER "\t"

// The number of memory for the ZC files' names
#define ZC_FILE_NAME_STRING_SIZE 2048

// The term which users look for in each ZC file
#define CONTENT_lOOKING_FOR "Channel Id"

// The term which users look for after CONTENT_lOOKING_FOR in each ZC file
#define CONTENT_lOOKING_FOR_DELIMITER ":"

// The postfix artificial name in the original ZC file name
#define POSIX_ARTIFICIAL_TERM ".zc"

// The size for reserving the number into an array in a string format
#define PACKET_NUMBER_ARRAY_SIZE 80

// The format of "PFRING_COMMAND" for calling the execution, pfcount
#ifdef _CmockaTest_
#define PFRING_COMMAND "sudo ../Imports/Exes/pfcountpfcount -i zc:"
#else
#define PFRING_COMMAND "sudo ./Imports/Exes/pfcountpfcount -i zc:"
#endif

// The NIC name
#define NIC_NAME "ens2f1"

typedef struct LibPFRingPacketLost LibPFRingPacketLost;
typedef struct interruptedControllingInformation interruptedControllingInformation;
/**
 * The LibPFRingPacketLost class definition
 */
struct LibPFRingPacketLost {
    // The file system object
    LibFileSystem* fileSystem;
    // Please refer to the function, execPacketLost
    void (*pf_execPacketLost)(LibPFRingPacketLost*);
};

/**
 * All information for the interrupt signal; including "jumping out the infinite loop for all child processes" information
 */
struct interruptedControllingInformation {
    short isBreakingMonitoredLoop;
};

void LibPFRingPacketLost_Construct(LibPFRingPacketLost*);
void LibPFRingPacketLost_Destruct(LibPFRingPacketLost*);