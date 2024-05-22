#pragma once
/** @file LibQueuingMachine
 * The Queuing Machine for obtaining number with multi-processing; this function will
 * be implemented by the semaphore mechanism in operating system because the author feels
 * like to make sure that each process has a unique number and this number will be corresponding
 * to the number of RSS
 *
 * @author Nick, Liao
 * @date 2024/04/25
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// This file (./DebugFileInclusion.h) is included from the other header file which is imported from the test header file; 
// if the macro "_CmockaTest_" is closed in the the test header, the file includes nothing
#include "./DebugFileInclusion.h"

typedef struct LibQueuingMachine LibQueuingMachine;
typedef struct SharedMemoryForQueue SharedMemoryForQueue;

/**
 * The queuing machine class
 */
struct LibQueuingMachine {
    // The structure for holding the data in the shared memory
    SharedMemoryForQueue* queueInstance;
    // Setting the power number into the object/instance
    void (*pf_execQueueingMachineNumber)(LibQueuingMachine*, const int);
    // Setting number value from the field, "number", of the object, "queueInstance"
    void (*pf_setNumber)(LibQueuingMachine*, const int);
    // Obtaining number value from the field, "number", of the object, "queueInstance", and then the value of the number
    // will be equal to the current value - 1
    int (*pf_getNumber)(LibQueuingMachine*);
    // The variable for controlling when verifying the result in function with printing in the Cmocka mode
    short isPrintedForCmocka;
};

/**
 * The data structure of "LibQueuingMachine" for the shared memory
 */
struct SharedMemoryForQueue {
    int number;  // Starting from the value, zero
};

void LibQueuingMachine_Construct(LibQueuingMachine*);
void LibQueuingMachine_Destruct(LibQueuingMachine*);