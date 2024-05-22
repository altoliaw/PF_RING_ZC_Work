#include "../Headers/LibStack.h"

static int allocateMemory(LibStack*, const unsigned int);


/**
 * Construction function
 * 
 * @param LibStackInstance LibStack* The stack object for recording the pointers which users' memory will be allocated manually
 * @return void
 */ 
void LibStack_Construct(LibStack* LibStack) 
{
	LibStack->startStack = NULL;
	LibStack->size = 0;
	LibStack->currentIndex = -1;
	LibStack->allocateMemory = &allocateMemory;
}

/**
 * Destruction function
 * 
 * @param LibStackInstance LibStack* The stack object for recording the pointers which users' memory will be allocated manually
 * @return void
 */
void LibStack_Destruct(LibStack* LibStackInstance) 
{
	if(LibStackInstance->startStack != NULL) {
		for(int i = 0; i < LibStackInstance->size; i++) {
			if((LibStackInstance->startStack)[i] != NULL) {
				LibStackElement_Destruct((LibStackInstance->startStack)[i]);

				#ifdef _CmockaTest_
				test_free((LibStackInstance->startStack)[i]);
				#else
				free((LibStackInstance->startStack)[i]);
				#endif

				(LibStackInstance->startStack)[i] = NULL;
			}
		}
		#ifdef _CmockaTest_
		test_free(LibStackInstance->startStack);
		#else
		free(LibStackInstance->startStack);
		#endif

		LibStackInstance->startStack = NULL;
		LibStackInstance->allocateMemory = NULL;
	}
}

/**
 * Construction
 * 
 * @param LibStackElementInstance 
 */
void LibStackElement_Construct(LibStackElement* LibStackElementInstance) 
{
	LibStackElementInstance->objectTypeName = NULL;
	LibStackElementInstance->category = None;
	LibStackElementInstance->stuff = NULL;
}

/**
 * Deconstruction
 * 
 * @param LibStackElementInstance 
 */
void LibStackElement_Destruct(LibStackElement* LibStackElementInstance)
{
	if(LibStackElementInstance != NULL) {
		switch(LibStackElementInstance->category) {
			case VARIABLE:
				if(strcmp(LibStackElementInstance->objectTypeName, "int*") == 0) {
					#ifdef _CmockaTest_
					test_free ((int*)(LibStackElementInstance->stuff));
					test_free(LibStackElementInstance->objectTypeName);
					#else
					free ((int*)(LibStackElementInstance->stuff));
					free(LibStackElementInstance->objectTypeName);
					#endif
				} else if (strcmp(LibStackElementInstance->objectTypeName, "float*") == 0) {
					#ifdef _CmockaTest_
					test_free ((float*)(LibStackElementInstance->stuff));
					test_free(LibStackElementInstance->objectTypeName);
					#else
					free ((float*)(LibStackElementInstance->stuff));
					free(LibStackElementInstance->objectTypeName);
					#endif
				}
				break;
			case FUNCTION:
				break;
		}
		LibStackElementInstance->stuff = NULL;
		LibStackElementInstance->objectTypeName = NULL;
	}
}

/**
 * Allocating memory manually
 * 
 * @param LibStack [LibStack*] The stack object for recording the pointers which users' memory will be allocated manually
 * @param size [const unsigned int] The number of the units for the allocated memory 
 * @return [int] The integer shows the situation; 0 : Success, 1 : Failure
 */
static int allocateMemory(LibStack* LibStackInstance, const unsigned int size)
{
	// If the the stack is NULL and the the size is equal to zero, "calloc" will be used.
	if(LibStackInstance->startStack == NULL && LibStackInstance->size <= 0 && LibStackInstance->size < size) {
		#ifdef _CmockaTest_
		LibStackInstance->startStack = test_calloc(size, sizeof(LibStackElement*));
		#else
		LibStackInstance->startStack = calloc(size, sizeof(LibStackElement*));
		#endif
		if(LibStackInstance->startStack == NULL){
			// Error occurrence
			return -1;
		}
		LibStackInstance->size = size;
		LibStackInstance->currentIndex = 0;

	} else if (LibStackInstance->startStack != NULL && LibStackInstance->size < size) { // The stack shall be resized.
		#ifdef _CmockaTest_
		LibStackInstance->startStack = test_realloc(LibStackInstance->startStack, sizeof(LibStackElement*) * size);
		#else
		LibStackInstance->startStack = realloc(LibStackInstance->startStack, sizeof(LibStackElement*) * size);
		#endif
		if(LibStackInstance->startStack == NULL) {
			// Error occurrence
			return -1;
		}
		LibStackInstance->size = size;
	}
	return 0;
}