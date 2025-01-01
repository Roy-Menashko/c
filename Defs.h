//
// Created by Avishur on 12/6/2024.
//

#ifndef DEFS_H
#define DEFS_H


/*
 * Header file defines basic types, enumerations, and function pointer types.
 */

/*
 * `#ifndef DEFS_H` and `#define DEFS_H`: Include guard to prevent multiple inclusions
 * of the header file in a single compilation unit.
 */

/*
 * `typedef enum e_bool { false, true } bool;`:
 * Defines a custom boolean type, `bool`, with values `false` (0) and `true` (1).
 */

/*
 * `typedef enum e_status { success, failure } status;`:
 * Enumerates function return statuses with two possible values:
 * `success` (indicates successful execution) and `failure` (indicates an error or failure).
 */

/*
 * `typedef void * Element;`:
 * Generic pointer type for elements, allowing flexibility in handling any data type.
 */

/*
 * `typedef Element(*CopyFunction) (Element);`:
 * Function pointer type for functions that copy an element and return the copied element.
 */

/*
 * `typedef status(*FreeFunction) (Element);`:
 * Function pointer type for functions that free the memory of an element
 * and return a status (`success` or `failure`).
 */

/*
 * `typedef status(*PrintFunction) (Element);`:
 * Function pointer type for functions that print an element and return a status.
 */

/*
 * `typedef int(*TransformIntoNumberFunction) (Element);`:
 * Function pointer type for functions that transform an element into an integer
 * (e.g., for use in hash functions).
 */

/*
 * `typedef bool(*EqualFunction) (Element, Element);`:
 * Function pointer type for functions that compare two elements for equality,
 * returning `true` if they are equal and `false` otherwise.
 */

/*
 * `#endif //DEFS_H`: Closes the include guard, ensuring the header file content
 * is processed only once during compilation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum e_bool { false, true } bool;
typedef enum e_status { success, failure } status;

typedef void * Element;

typedef Element(*CopyFunction) (Element);
typedef status(*FreeFunction) (Element);
typedef status(*PrintFunction) (Element);
typedef int(*TransformIntoNumberFunction) (Element);
typedef bool(*EqualFunction) (Element, Element);

#endif //DEFS_H
