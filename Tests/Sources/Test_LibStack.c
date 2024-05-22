#include "../Headers/Test_LibStack.h"

/**
 * Verifying the two elements where the data in two elements have the same stuffs
 * (point to the same anonymous location) and the constructor of "LibStackElement_Construct"
 *
 * @param State void** None
 */
void Test_LibStack_insertElementsToStackProcess(void** State) {
    int* dynamicValue = test_calloc(1, sizeof(int));
    *dynamicValue = 12;
    char* typeName = test_calloc(strlen("int*") + 1, sizeof(char));
    memcpy(typeName, "int*", strlen("int*"));
    typeName[strlen("int*")] = '\0';

    // Generating the element 1
    LibStackElement element;
    element.category = VARIABLE;
    element.stuff = (void*)(dynamicValue);
    element.objectTypeName = typeName;

    // Generating the element 2
    LibStackElement* element2 = test_calloc(1, sizeof(LibStackElement));
    LibStackElement_Construct(element2);
    element2->category = VARIABLE;
    element2->stuff = (void*)dynamicValue;
    element2->objectTypeName = typeName;

    assert_int_equal(*((int*)element.stuff), *dynamicValue);
    assert_int_equal(*((int*)element2->stuff), *dynamicValue);
    assert_ptr_equal(((int*)element.stuff), ((int*)element2->stuff));

    if (dynamicValue != NULL) {
        test_free(dynamicValue);
    }
    if (typeName != NULL) {
        test_free(typeName);
    }
    if (dynamicValue != NULL) {
        test_free(element2);
    }
}

/**
 * Verifying the two elements where the data in two elements have the same stuffs
 * (point to the same anonymous location), and the constructor, "LibStackElement_Construct",
 * and destructor, "LibStackElement_Destruct"
 *
 * @param State void** None
 */
void Test_LibStack_insertElementsToStackProcess2(void** State) {
    int* dynamicValue = test_calloc(1, sizeof(int));
    *dynamicValue = 12;
    char* typeName = test_calloc(strlen("int*") + 1, sizeof(char));
    memcpy(typeName, "int*", strlen("int*"));
    typeName[strlen("int*")] = '\0';

    // Generating the element
    LibStackElement* element = test_calloc(1, sizeof(LibStackElement));
    LibStackElement_Construct(element);
    element->category = VARIABLE;
    element->stuff = (void*)dynamicValue;
    element->objectTypeName = typeName;

    assert_int_equal(*((int*)element->stuff), *dynamicValue);
    assert_string_equal(element->objectTypeName, typeName);
    LibStackElement_Destruct(element);
    assert_null(((int*)element->stuff));
    assert_null(((int*)element->objectTypeName));
    if (element != NULL) {
        test_free(element);
    }
}

/**
 * Checking the constructor of "LibStack_Construct"
 *
 * @param State void** None
 */
void Test_LibStack_insertElementsToStackProcess3(void** State) {
    int* dynamicValue = test_calloc(1, sizeof(int));
    *dynamicValue = 12;
    char* typeName = test_calloc(strlen("int*") + 1, sizeof(char));
    memcpy(typeName, "int*", strlen("int*"));
    typeName[strlen("int*")] = '\0';

    // Generating the element
    LibStackElement* element = test_calloc(1, sizeof(LibStackElement));
    LibStackElement_Construct(element);
    element->category = VARIABLE;
    element->stuff = (void*)(dynamicValue);
    element->objectTypeName = typeName;

    LibStack* stack = test_calloc(1, sizeof(LibStack));
    LibStack_Construct(stack);
    int size = 25;
    stack->startStack = test_calloc(size, sizeof(LibStackElement*));
    (stack->startStack)[0] = element;
    stack->size = size;
    stack->currentIndex = 0;

    assert_int_equal(*((int*)element->stuff), *dynamicValue);
    assert_string_equal(element->objectTypeName, typeName);
    assert_int_equal(*((int*)((stack->startStack)[0])->stuff), *dynamicValue);
    assert_string_equal(((stack->startStack)[0])->objectTypeName, typeName);

    if(stack->startStack != NULL) {
		for(int i = 0; i < stack->size; i++) {
			if((stack->startStack)[i] != NULL) {
				LibStackElement_Destruct((stack->startStack)[i]);
				test_free((stack->startStack)[i]);
				(stack->startStack)[i] = NULL;
			}
		}
		test_free(stack->startStack);
		stack->startStack = NULL;
		stack->allocateMemory = NULL;
	}
    
    if (stack != NULL) {
        test_free(stack);
    }
}

/**
 * Verifying if the function, allocateMemory, is correct
 * 
 * @param State void** None
 */
void Test_LibStack_insertElementsToStackProcess4(void** State) {
    int* dynamicValue = test_calloc(1, sizeof(int));
    *dynamicValue = 12;
    char* typeName = test_calloc(strlen("int*") + 1, sizeof(char));
    memcpy(typeName, "int*", strlen("int*"));
    typeName[strlen("int*")] = '\0';

    // Generating the element
    LibStackElement* element = test_calloc(1, sizeof(LibStackElement));
    LibStackElement_Construct(element);
    element->category = VARIABLE;
    element->stuff = (void*)(dynamicValue);
    element->objectTypeName = typeName;

    LibStack* stack = test_calloc(1, sizeof(LibStack));
    LibStack_Construct(stack);
    int size = 25;
    stack->allocateMemory(stack, (unsigned int)size);
    (stack->startStack)[0] = element;

    assert_int_equal(*((int*)element->stuff), *dynamicValue);
    assert_string_equal(element->objectTypeName, typeName);
    assert_int_equal(*((int*)((stack->startStack)[0])->stuff), *dynamicValue);
    assert_string_equal(((stack->startStack)[0])->objectTypeName, typeName);

    if(stack->startStack != NULL) {
		for(int i = 0; i < stack->size; i++) {
			if((stack->startStack)[i] != NULL) {
				LibStackElement_Destruct((stack->startStack)[i]);
				test_free((stack->startStack)[i]);
				(stack->startStack)[i] = NULL;
			}
		}
		test_free(stack->startStack);
		stack->startStack = NULL;
		stack->allocateMemory = NULL;
	}
    
    if (stack != NULL) {
        test_free(stack);
    }
}

/**
 * Verifying if the destructor, LibStack_Destruct, is correct
 * 
 * @param State void** None
 */
void Test_LibStack_insertElementsToStackProcess5(void** State) {
    int* dynamicValue = test_calloc(1, sizeof(int));
    *dynamicValue = 12;
    char* typeName = test_calloc(strlen("int*") + 1, sizeof(char));
    memcpy(typeName, "int*", strlen("int*"));
    typeName[strlen("int*")] = '\0';

    // Generating the element
    LibStackElement* element = test_calloc(1, sizeof(LibStackElement));
    LibStackElement_Construct(element);
    element->category = VARIABLE;
    element->stuff = (void*)(dynamicValue);
    element->objectTypeName = typeName;

    LibStack* stack = test_calloc(1, sizeof(LibStack));
    LibStack_Construct(stack);
    int size = 25;
    stack->allocateMemory(stack, (unsigned int)size);
    (stack->startStack)[0] = element;

    assert_int_equal(*((int*)element->stuff), *dynamicValue);
    assert_string_equal(element->objectTypeName, typeName);
    assert_int_equal(*((int*)((stack->startStack)[0])->stuff), *dynamicValue);
    assert_string_equal(((stack->startStack)[0])->objectTypeName, typeName);

    LibStack_Destruct(stack);
    
    if (stack != NULL) {
        test_free(stack);
    }
}