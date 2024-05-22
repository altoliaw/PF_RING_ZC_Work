#pragma once
/** @file Test_LibFileSystem.h
 * File system implementation
 *
 * @author Nick Liao
 * @date 2024/04/30
 */
#include <stdio.h>  // This variable is for including the "cmocka's" header files.
#include <regex.h> // The header of the regular expression

#include "../../Headers/LibFileSystem.h"
#include "./DebugFileInclusion.h"

void Test_LibFileSystem_FindFilesProcess(void**);
void Test_LibFileSystem_FindFilesProcess2(void**);
void Test_LibFileSystem_FindFilesProcess3(void**);
void Test_LibFileSystem_FindFilesProcess4(void**);
void Test_LibFileSystem_FindFilesProcess5(void**);