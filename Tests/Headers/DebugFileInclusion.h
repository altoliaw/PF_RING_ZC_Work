#pragma once
/** @file DebugFileInclusion
 * This page contains all macro variables for debugging
 *
 * @author Nick, Liao
 * @date 2024/04/25
 */

// This section contains the header files and is used to the Cmocka's project.
#ifdef _CmockaTest_
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
// The included file "cmocka.h" shall be put at last
#include <cmocka.h>
#include <string.h>

// Declaration of the array result size for Cmocka's unit tests and this variable is used for "DebugFileInclusion.h"
#define _Buffer_Size_ 100000

// Recording the file name for Cmocka's unit tests; the variable is defined in "MainHeader.h"
extern char _System_Test_Cwd[200];

// Obtaining the cmake test's pwd (e.g., "/c/Users/Nick/Documents/Workspace/PfringZCInfo/build/Tests")
// where "PfringZCInfo" is the project name and "Tests" shows the folder for unit tests
#define _PWD_                                                             \
    do {                                                                  \
        if (getcwd(_System_Test_Cwd, sizeof(_System_Test_Cwd)) == NULL) { \
            fprintf(stderr, "getcwd(.) failed\n");                        \
            exit(-1);                                                     \
        }                                                                 \
    } while (0)

// Changing the stdout for unit testing where the type of origin_stdout is int*
#define _Change_Stdout_Start_(origin_stdout)                                            \
    do {                                                                                \
        fflush(stdout);                                                                 \
        if (strlen(_System_Test_Cwd) <= 0) {                                            \
            _PWD_;                                                                      \
            sprintf((_System_Test_Cwd + strlen(_System_Test_Cwd)),                      \
                    "/tmp_%08d.txt", getpid());                                         \
        }                                                                               \
        int fileDescriptor = open(_System_Test_Cwd, O_RDWR | O_CREAT | O_APPEND, 0666); \
        *origin_stdout = dup(STDOUT_FILENO);                                            \
        if (dup2(fileDescriptor, STDOUT_FILENO) < 0) {                                  \
            fprintf(stderr, "STDOUT redirection failed\n");                             \
            exit(-1);                                                                   \
        }                                                                               \
        close(fileDescriptor);                                                          \
    } while (0)

#define _Change_Stdout_End_(origin_stdout, result, resultSize)                   \
    do {                                                                         \
        fflush(stdout);                                                          \
        if (strlen(_System_Test_Cwd) <= 0) {                                     \
            fprintf(stderr, "_System_Test_Cwd does not exist!\n");               \
            exit(-1);                                                            \
        }                                                                        \
        FILE* file = fopen(_System_Test_Cwd, "rb");                              \
        if (file == NULL) {                                                      \
            fprintf(stderr, "The file from _System_Test_Cwd dose not exist!\n"); \
            exit(-1);                                                            \
        }                                                                        \
        fread(result, sizeof(char), resultSize, file);                           \
        fclose(file);                                                            \
        remove(_System_Test_Cwd);                                                \
        /*Redirect STDOUT to STDOUT_FILENO*/                                     \
        if (dup2(*origin_stdout, STDOUT_FILENO) < 0) {                           \
            fprintf(stderr, "STDOUT redirection failed\n");                      \
            exit(-1);                                                            \
        }                                                                        \
        close(*origin_stdout);                                                   \
    } while (0)
#endif
