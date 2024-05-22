#pragma once
/** @file LibStack.h
 * A structure for handling the stack information
 * 
 * @author Nick, Liao
 * @date 2024/04/23
 */
#include <stdlib.h>
#include <string.h>
#include "./DebugFileInclusion.h"

typedef enum stackCategory stackCategory;
typedef struct LibStackElement LibStackElement;
typedef struct LibStack LibStack;
/**
 * The definition of the category which the pointer points to 
 */
typedef enum stackCategory stackCategory;

/**
 * The enumeration
 */
enum stackCategory {
	None = -1,
	VARIABLE = 0, // a variable
	FUNCTION // a function
}; 

/**
 * The definition of the stack for releasing the memory by users' manually allocated
 */
struct LibStackElement {
    void* stuff; // The object that the pointer refer to
	stackCategory category; // The object category
	char* objectTypeName; // Using for recording the user-defined type name
};

/**
 * The definition of the stack for releasing the memory by users' manually allocated
 */
struct LibStack {
	// Starting pointer to an array, each element in the array reserves the pointer of the LibStackElement object
    LibStackElement** startStack;
	//The current index;
	int currentIndex;
	//The array sizes;
	int size;
	// The function pointer points to the function which will allocate memory manually
	int (*allocateMemory)(LibStack*, unsigned int);
};

// Construction/Destruction functions
void LibStack_Construct(LibStack*); // Construction
void LibStack_Destruct(LibStack*); // Destruction
void LibStackElement_Construct(LibStackElement*); // Construction
void LibStackElement_Destruct(LibStackElement*); // Destruction