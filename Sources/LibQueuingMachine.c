#include "../Headers/LibQueuingMachine.h"

static void execQueueingMachineNumber(LibQueuingMachine*, const int);
static void setNumber(LibQueuingMachine*, const int);
static int getNumber(LibQueuingMachine*);

/**
 * Constructor
 *
 * @param object LibQueuingMachine's object
 */
void LibQueuingMachine_Construct(LibQueuingMachine* object) {
    object->isPrintedForCmocka = 0;
    object->queueInstance = NULL;
#ifdef _CmockaTest_
    object->queueInstance = test_calloc(1, sizeof(SharedMemoryForQueue));
#else
    object->queueInstance = calloc(1, sizeof(SharedMemoryForQueue));
#endif
    object->pf_execQueueingMachineNumber = &execQueueingMachineNumber;
    object->pf_setNumber = &setNumber;
    object->pf_getNumber = &getNumber;
}

/**
 * Destructor
 *
 * @param object LibQueuingMachine's object
 */
void LibQueuingMachine_Destruct(LibQueuingMachine* object) {
    if (object->queueInstance != NULL) {
#ifdef _CmockaTest_
        test_free(object->queueInstance);
#else
        free(object->queueInstance);
#endif
    }
}

/**
 * Generating the queueing machine number into the object
 *
 * @param object [LibQueuingMachine*] The called object itself
 * @param powerNumber [const int] The total number of the RSS
 */
static void execQueueingMachineNumber(LibQueuingMachine* object, const int powerNumber) {
    object->queueInstance->number = powerNumber;

#ifdef _CmockaTest_
    if(object->isPrintedForCmocka == 1){
        // This printf will be printed to the custom file and the "printf" does not show on the terminal.
        printf("%d\n", object->queueInstance->number);
    }
#endif
}

/**
 * Setting the index of the number of the RSS to the shared memory; the function is
 * implemented by the semaphore; this function only focuses on the critical section of the semaphore
 *
 * @param object [LibQueuingMachine*] The LibQueuingMachine object itself
 * @param number [const int] The number for the shared memory
 * shows the status when calling the shared memory
 * @return [void]
 */
static void setNumber(
    LibQueuingMachine* object,
    const int number) {
    object->queueInstance->number = number;
}

/**
 * Returning the index of the number of the RSS from the shared memory; the function is implemented by the semaphore
 *
 * @param object [LibQueuingMachine*] The LibQueuingMachine object itself
 * @return [int] The index of the number of the RSS if the function is success; otherwise, -1
 */
static int getNumber(LibQueuingMachine* object) {
    int result  = object->queueInstance->number;
    setNumber(object, object->queueInstance->number - 1);
    return result;
}