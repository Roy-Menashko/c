#include <stdio.h>
#include <stdlib.h>
#include "HashTable.h"
#include "LinkedList.h"
#include "KeyValuePair.h"

struct hashTable_s {
    int size;                                  // The number of buckets in the hash table
    FreeFunction free_key;                    // Function to free the memory of a key
    FreeFunction free_value;                  // Function to free the memory of a value
    PrintFunction print_key;                  // Function to print a key
    PrintFunction print_value;                // Function to print a value
    CopyFunction copy_key;                    // Function to copy a key
    CopyFunction copy_value;                  // Function to copy a value
    EqualFunction equal_key;                  // Function to compare two keys for equality
    EqualFunction equal_value;                // Function to compare two values for equality
    TransformIntoNumberFunction transformIntoNumber; // Function to transform a key into a numeric value (hashing)
    LinkedList* buckets;                      // Array of linked lists, where each bucket stores key-value pairs
};

static status freeKeyValuePairWrapper(Element e) {
    if(!e) return failure;
    // Cast the generic Element to a KeyValuePair and call destroyKeyValuePair
    destroyKeyValuePair((KeyValuePair)e);
    return success;
}

static bool compareKeyValuePairWrapper(Element e1, Element e2) {
    // Cast the first generic Element to a KeyValuePair
    KeyValuePair p1 = (KeyValuePair)e1;

    // Cast the second generic Element to a KeyValuePair
    KeyValuePair p2 = (KeyValuePair)e2;

    // Use isEqualKey to compare the key of the first pair with the key of the second pair
    return isEqualKey(p1, getKey(p2));
}


static status printKeyValuePairWrapper(Element e) {
    if(!e) return failure;
    // Cast the generic Element to a KeyValuePair
    // Call displayKeyValue to print both the key and value of the pair
    displayKeyValue((KeyValuePair)e);
    return success;
}


hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey,
                          CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue,
                          EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber,
                          int hashNumber) {
    // Validate input function pointers and hash table size
    if (!copyKey || !freeKey || !printKey || !copyValue || !freeValue || !printValue ||
        !equalKey || !transformIntoNumber) {
        return NULL; // Return NULL if any required function is missing
    }

    // Allocate memory for the hash table structure
    hashTable newTable = (hashTable)malloc(sizeof(*newTable));
    if (!newTable) { // Check if memory allocation failed
        return NULL; // Return NULL if unable to allocate memory
    }

    // Initialize the hash table fields
    newTable->size = hashNumber;                       // Set the number of buckets
    newTable->copy_key = copyKey;                      // Assign the key copy function
    newTable->free_key = freeKey;                      // Assign the key free function
    newTable->print_key = printKey;                    // Assign the key print function
    newTable->copy_value = copyValue;                  // Assign the value copy function
    newTable->free_value = freeValue;                  // Assign the value free function
    newTable->print_value = printValue;                // Assign the value print function
    newTable->equal_key = equalKey;                    // Assign the key comparison function
    newTable->equal_value = NULL;                      // (Optional) Value comparison function, unused here
    newTable->transformIntoNumber = transformIntoNumber; // Assign the hashing function

    // Allocate memory for the buckets (array of LinkedList pointers)
    newTable->buckets = (LinkedList*)malloc(hashNumber * sizeof(LinkedList));
    if (!newTable->buckets) { // Check if memory allocation for buckets failed
        free(newTable);       // Free the hash table structure
        return NULL;          // Return NULL to indicate failure
    }

    // Initialize each bucket with a linked list
    for (int i = 0; i < hashNumber; i++) {
        newTable->buckets[i] = createLinkedList(freeKeyValuePairWrapper, compareKeyValuePairWrapper, printKeyValuePairWrapper);
        if (!newTable->buckets[i]) { // Check if LinkedList creation failed
            destroyHashTable(newTable); // Destroy the hash table and free allocated memory
            return NULL;                // Return NULL to indicate failure
        }
    }

    return newTable; // Return the newly created hash table
}

int findIndex(hashTable table, Element key) {
    int hash = table->transformIntoNumber(key);
    return hash % table->size;
}


status destroyHashTable(hashTable table) {
    // Check if the hash table is NULL
    if (table == NULL) {
        return failure; // Return failure if the table is already NULL
    }

    // Loop through each bucket in the hash table
    for (int i = 0; i < table->size; i++) {
        // Destroy the linked list in each bucket
        destroyLinkedList(table->buckets[i]);
    }

    // Free the memory allocated for the buckets array
    free(table->buckets);

    // Free the memory allocated for the hash table structure itself
    free(table);

    // Set the table pointer to NULL to avoid dangling references
    table = NULL;

    return success; // Return success to indicate the table was successfully destroyed
}


status addToHashTable(hashTable table, Element key, Element value) {
    // Validate the inputs
    if (!table || !key || !value) {
        return failure; // Return failure if the table, key, or value is NULL
    }

    /* Create copies of the key and value using the provided copy functions */
    Element keyCopy = table->copy_key(key); // Create a copy of the key
    if (!keyCopy) { // Check if key copying failed
        return failure; // Return failure if unable to copy the key
    }

    Element valueCopy = table->copy_value(value); // Create a copy of the value
    if (!valueCopy) { // Check if value copying failed
        table->free_key(keyCopy); // Free the key copy to prevent memory leaks
        return failure; // Return failure if unable to copy the value
    }

    /* Create a KeyValuePair object to store the key and value */
    KeyValuePair pair = createKeyValuePair(
        keyCopy,
        valueCopy,
        table->free_key,
        table->free_value,
        table->print_key,
        table->print_value,
        table->equal_key
    );
    if (!pair) { // Check if KeyValuePair creation failed
        /* If creation fails, free the allocated key and value */
        table->free_key(keyCopy);
        table->free_value(valueCopy);
        return failure; // Return failure if unable to create the KeyValuePair
    }

    /* Calculate the hash index for the key */
    int hash = findIndex(table, key);
    /* The hash index determines the bucket where the KeyValuePair will be stored */

    /* Add the KeyValuePair to the appropriate bucket (linked list) */
    if (appendNode(table->buckets[hash], pair) == failure) { // Check if insertion failed
        destroyKeyValuePair(pair); // Free the KeyValuePair to prevent memory leaks
        return failure; // Return failure if unable to append the node
    }

    return success; // Return success to indicate the key-value pair was added
}

Element lookupInHashTable(hashTable table, Element key) {
    // Validate the inputs
    if (!table || !key) {
        return NULL; // Return NULL if the hash table or key is invalid
    }

    // Calculate the hash index for the key
    int hash = findIndex(table, key);
    // Ensure the hash value is within the valid range using modulo operation

    // Get the linked list (bucket) at the computed hash index
    LinkedList bucket = table->buckets[hash];
    if (!bucket) {
        return NULL; // Return NULL if the bucket does not exist
    }

    // Start traversing the bucket to find the key-value pair
    Element e = getFirstElement(bucket); // Get the first element in the bucket
    while (e) {
        // Cast the element to KeyValuePair
        KeyValuePair p = (KeyValuePair)e;

        // Check if the key in the pair matches the lookup key
        if (table->equal_key(getKey(p), key)) {
            return getValue(p); // Return the value if the keys match
        }

        // Move to the next element in the bucket
        e = getNextElement(bucket, e);
    }

    // Return NULL if the key is not found in the bucket
    return NULL;
}


status removeFromHashTable(hashTable table, Element key) {
    // Validate the inputs
    if (!table || !key) {
        return failure; // Return failure if the hash table or key is invalid
    }

    // Calculate the hash index for the key
    int hash = findIndex(table, key);
    // Ensure the hash value is within the valid range using modulo operation

    // Get the linked list (bucket) at the computed hash index
    LinkedList bucket = table->buckets[hash];
    if (!bucket) {
        return failure; // Return failure if the bucket does not exist
    }

    // Start traversing the bucket to find the key-value pair
    Element e = getFirstElement(bucket); // Get the first element in the bucket
    while (e) {
        // Cast the element to KeyValuePair
        KeyValuePair p = (KeyValuePair)e;

        // Check if the key in the pair matches the key to be removed
        if (table->equal_key(getKey(p), key)) {
            // Remove the node from the linked list
            if (deleteNode(bucket, e) == failure) {
                return failure; // Return failure if the deletion operation fails
            }
            return success; // Return success after removing the key-value pair
        }

        // Move to the next element in the bucket
        e = getNextElement(bucket, e);
    }

    // Return failure if the key was not found in the bucket
    return failure;
}


status displayHashElements(hashTable table) {
    // Validate the hash table
    if (!table) {
        return failure; // Return failure if the table is NULL
    }

    // Iterate through all buckets in the hash table
    for (int i = 0; i < table->size; i++) {
        // Display the elements in the linked list (bucket) at index i
        displayList(table->buckets[i]);
    }

    return success; // Return success after displaying all elements
}


