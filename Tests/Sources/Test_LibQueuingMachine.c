#include "../Headers/Test_LibQueuingMachine.h"

/**
 * Verifying if the constructor is correct
 *
 * @param state
 */
void Test_LibQueuingMachine_insertElementsProcess(void** state) {
    LibQueuingMachine* queue = test_calloc(1, sizeof(LibQueuingMachine));
    test_free(queue);
}

/**
 * Verifying if the constructor is correct
 *
 * @param state
 */
void Test_LibQueuingMachine_insertElementsProcess2(void** state) {
    LibQueuingMachine* queue = test_calloc(1, sizeof(LibQueuingMachine));
    LibQueuingMachine_Construct(queue);
    test_free(queue->queueInstance);
    test_free(queue);
}

/**
 * Verifying if the constructor is correct
 *
 * @param state
 */
void Test_LibQueuingMachine_insertElementsProcess3(void** state) {
    LibQueuingMachine* queue = test_calloc(1, sizeof(LibQueuingMachine));
    LibQueuingMachine_Construct(queue);
    LibQueuingMachine_Destruct(queue);
    test_free(queue);
}

/**
 * Verifying the function of "void (*pf_generateQueueingMachineNumber)(LibQueuingMachine*, const int)"
 *
 * @param state
 */
void Test_LibQueuingMachine_insertElementsProcess4(void** state) {
    int origin_stdout = -1;
    char buffer[_Buffer_Size_];
    LibQueuingMachine* queue = test_calloc(1, sizeof(LibQueuingMachine));
    LibQueuingMachine_Construct(queue);
    queue->isPrintedForCmocka = 1; // Opening the print function for the verified function
    int number = 16;

    _Change_Stdout_Start_(&origin_stdout);
    queue->pf_execQueueingMachineNumber(queue, number);
    _Change_Stdout_End_(&origin_stdout, buffer, _Buffer_Size_);

    assert_int_equal(atoi(buffer), number);

    LibQueuingMachine_Destruct(queue);
    test_free(queue);
}

/**
 * Verifying the function of "int (*pf_getQueueingMachineNumber)(LibQueuingMachine*)" and the
 * returned value from the function is equal to ${number}
 *
 * @param state
 */
void Test_LibQueuingMachine_insertElementsProcess5(void** state) {
    int origin_stdout = -1;
    char buffer[_Buffer_Size_];
    LibQueuingMachine* queue = test_calloc(1, sizeof(LibQueuingMachine));
    LibQueuingMachine_Construct(queue);
    queue->isPrintedForCmocka = 1; // Opening the print function for the verified function
    int number = 16;

    _Change_Stdout_Start_(&origin_stdout);
    queue->pf_execQueueingMachineNumber(queue, number);
    _Change_Stdout_End_(&origin_stdout, buffer, _Buffer_Size_);

    assert_int_equal(atoi(buffer), number);

    number = 40;
    queue->pf_setNumber(queue, number);
    assert_int_equal(queue->queueInstance->number, number);
    LibQueuingMachine_Destruct(queue);
    test_free(queue);
}

/**
 * Verifying the function of "int (*pf_getQueueingMachineNumber)(LibQueuingMachine*)" and the
 * returned value from the function is equal to ${number} - 1
 *
 * @param state
 */
void Test_LibQueuingMachine_insertElementsProcess6(void** state) {
    int origin_stdout = -1;
    char buffer[_Buffer_Size_];
    LibQueuingMachine* queue = test_calloc(1, sizeof(LibQueuingMachine));
    LibQueuingMachine_Construct(queue);
    queue->isPrintedForCmocka = 1; // Opening the print function for the verified function
    int number = 16;

    _Change_Stdout_Start_(&origin_stdout);
    queue->pf_execQueueingMachineNumber(queue, number);
    _Change_Stdout_End_(&origin_stdout, buffer, _Buffer_Size_);

    assert_int_equal(atoi(buffer), number);

    number = 40;
    queue->pf_setNumber(queue, number);
    assert_int_equal(queue->pf_getNumber(queue), number);
    assert_int_equal(queue->queueInstance->number, number - 1);
    LibQueuingMachine_Destruct(queue);
    test_free(queue);
}