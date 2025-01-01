//
// Created by Avishur on 12/22/2024.
// This header file defines the interface for a MultiValue Hash Table, which supports multiple values for a single key.
//

#ifndef MULTIVALUEHASHTABLE_H
#define MULTIVALUEHASHTABLE_H

#include "Defs.h"        // Includes definitions of required types and functions.
#include "HashTable.h"   // Includes the base hash table functionality.

typedef struct MultiHashTable *hashTableProMax;
// Defines a pointer to the structure representing the MultiValue Hash Table.
// The actual structure is hidden (opaque pointer), ensuring encapsulation.

//
// Function Prototypes
//

// Creates a MultiValue Hash Table.
// Parameters:
// - copyKey: Function pointer to copy keys.
// - freeKey: Function pointer to free keys.
// - printKey: Function pointer to print keys.
// - copyValue: Function pointer to copy values.
// - freeValue: Function pointer to free values.
// - printValue: Function pointer to print values.
// - equalKey: Function pointer to compare keys for equality.
// - equal_value: Function pointer to compare values for equality.
// - transformIntoNumber: Function pointer to hash a key into an integer.
// - hashNumber: Size of the hash table (number of buckets).
// Returns a pointer to the newly created hash table or NULL on failure.
hashTableProMax createHashTableProMax(
    CopyFunction copyKey,
    FreeFunction freeKey,
    PrintFunction printKey,
    CopyFunction copyValue,
    FreeFunction freeValue,
    PrintFunction printValue,
    EqualFunction equalKey,
    EqualFunction equal_value,
    TransformIntoNumberFunction transformIntoNumber,
    int hashNumber
);

// Destroys the MultiValue Hash Table and releases all allocated memory.
// Parameters:
// - table: Pointer to the hash table to destroy.
// Returns a status code indicating success or failure.
status destroyHashTableProMax(hashTableProMax table);

// Adds a key-value pair to the MultiValue Hash Table.
// Parameters:
// - hashTableProMax: Pointer to the hash table.
// - key: The key to add or associate a value with.
// - value: The value to associate with the key.
// Returns a status code indicating success or failure.
status addToHashTableProMax(hashTableProMax hashTableProMax, Element key, Element value);

// Looks up values associated with a key in the MultiValue Hash Table.
// Parameters:
// - hashTableProMax: Pointer to the hash table.
// - key: The key to search for.
// Returns a pointer to the values associated with the key or NULL if the key is not found.
Element lookupInHashTableProMax(hashTableProMax hashTableProMax, Element key);

// Removes a specific key-value pair from the MultiValue Hash Table.
// Parameters:
// - hashTableProMax: Pointer to the hash table.
// - key: The key whose associated value needs to be removed.
// - value: The specific value to remove.
// Returns a status code indicating success or failure.
status removeFromHashTableProMax(hashTableProMax hashTableProMax, Element key, Element value);

// Displays all values associated with a specific key in the MultiValue Hash Table.
// Parameters:
// - hashTableProMax: Pointer to the hash table.
// - key: The key whose associated values are to be displayed.
// Returns a status code indicating success or failure.
status displayHashTableProMaxElementsByKey(hashTableProMax hashTableProMax, Element key);

#endif // MULTIVALUEHASHTABLE_H

