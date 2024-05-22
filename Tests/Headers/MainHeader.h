#pragma once
/** @file MainHeader.h
 * The main header file; definition of the included files and the testing groups
 *
 * @author Nick
 * @date 2024/04/23
 */
// Recording the file name for Cmocka's unit tests and this variable is used for "DebugFileInclusion.h"
char _System_Test_Cwd[200] = {'\0'};

#include "./DebugFileInclusion.h"
#include "./Test_LibStack.h"
const struct CMUnitTest Test_LibStack_Group[] = {
    cmocka_unit_test(Test_LibStack_insertElementsToStackProcess),
    cmocka_unit_test(Test_LibStack_insertElementsToStackProcess2),
    cmocka_unit_test(Test_LibStack_insertElementsToStackProcess3),
    cmocka_unit_test(Test_LibStack_insertElementsToStackProcess4),
    cmocka_unit_test(Test_LibStack_insertElementsToStackProcess5),
};


#include "./Test_LibQueuingMachine.h"
const struct CMUnitTest Test_LibQueuingMachine_Group[] = {
    cmocka_unit_test(Test_LibQueuingMachine_insertElementsProcess),
    cmocka_unit_test(Test_LibQueuingMachine_insertElementsProcess2),
    cmocka_unit_test(Test_LibQueuingMachine_insertElementsProcess3),
    cmocka_unit_test(Test_LibQueuingMachine_insertElementsProcess4),
    cmocka_unit_test(Test_LibQueuingMachine_insertElementsProcess5),
    cmocka_unit_test(Test_LibQueuingMachine_insertElementsProcess6),
};


#include "./Test_LibPFRingPacketLost.h"
const struct CMUnitTest Test_LibPFRingPacketLost_Group[] = {
    cmocka_unit_test(Test_LibPFRingPacketLost_insertMultiProcessProcess),
};

#include "./Test_LibFileSystem.h"
const struct CMUnitTest Test_LibFileSystem_Group[] = {
    cmocka_unit_test(Test_LibFileSystem_FindFilesProcess),
    cmocka_unit_test(Test_LibFileSystem_FindFilesProcess2),
    cmocka_unit_test(Test_LibFileSystem_FindFilesProcess3),
    cmocka_unit_test(Test_LibFileSystem_FindFilesProcess4),
    cmocka_unit_test(Test_LibFileSystem_FindFilesProcess5),
};