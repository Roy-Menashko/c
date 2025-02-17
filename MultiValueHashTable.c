#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MultiValueHashTable.h"
#include "LinkedList.h"
#include "KeyValuePair.h"




static status printListAsVoid(Element listPtr) {
    // Check if the input pointer 'listPtr' is NULL.
    // If it is NULL, return 'failure' to indicate an error.
    if (!listPtr) return failure;

    // Cast the generic 'Element' type (void*) to 'LinkedList'.
    // This assumes that 'listPtr' is a valid pointer to a LinkedList object.
    LinkedList list = (LinkedList)listPtr;

    // Call 'displayList' to print the contents of the LinkedList.
    // This function is expected to handle the actual display logic.
    return displayList(list);
}


static Element copyListAsVoid(Element listPtr) {
    // Check if the input pointer 'listPtr' is NULL.
    // If it is NULL, return NULL to indicate no copy can be made.
    if (!listPtr) return NULL;

    // Here, we are returning the same pointer without creating a new copy.
    // This means no actual duplication of the list is performed.
    return listPtr;
}


static status destroyListWrapper(Element listPtr) {
    // Check if the input pointer 'listPtr' is NULL.
    // If it is NULL, return 'failure' to indicate an invalid input.
    if (!listPtr) return failure;

    // Cast the generic 'Element' type (likely void*) to a 'LinkedList' type.
    // This assumes that 'listPtr' is a valid pointer to a LinkedList.
    // Call the 'destroyLinkedList' function to free all memory associated with the linked list.
    return destroyLinkedList((LinkedList)listPtr);
}


struct MultiHashTable {
    hashTable hashTable;
    // The main hash table that stores keys and their associated values.
    // This is typically implemented as a standard hash table but designed to handle multiple values per key.

    CopyFunction copy_key;
    // Function pointer to copy keys.
    // Used when adding new keys to ensure the keys are properly duplicated.

    FreeFunction free_key;
    // Function pointer to free keys.
    // Ensures proper memory management by freeing dynamically allocated keys when no longer needed.

    PrintFunction print_key;
    // Function pointer to print keys.
    // Used for debugging or displaying the keys in the hash table.

    CopyFunction copy_value;
    // Function pointer to copy values.
    // Ensures values are duplicated correctly when adding new associations.

    FreeFunction free_value;
    // Function pointer to free values.
    // Handles the memory deallocation of values stored in the hash table.

    PrintFunction print_value;
    // Function pointer to print values.
    // Used for debugging or displaying the values associated with keys.

    EqualFunction equal_key;
    // Function pointer to compare keys for equality.
    // Determines if two keys are logically the same, which is crucial for hashing.

    EqualFunction equal_value;
    // Function pointer to compare values for equality.
    // Helps to manage duplicate values and ensures accurate value operations.

    TransformIntoNumberFunction transformIntoNumber;
    // Function pointer to hash a key into a numeric value.
    // This numeric value is used to determine the bucket where the key-value pair is stored.
};




hashTableProMax createHashTableProMax(
    CopyFunction copyKey,
    FreeFunction freeKey,
    PrintFunction printKey,
    CopyFunction copyValue,
    FreeFunction freeValue,
    PrintFunction printValue,
    EqualFunction equalKey,
    EqualFunction equalValue,
    TransformIntoNumberFunction transformIntoNumber,
    int hashNumber
) {
    // Validate input parameters.
    // Ensure none of the function pointers are NULL and that hashNumber is greater than 0.
    if (!copyKey || !freeKey || !printKey ||
        !copyValue || !freeValue || !printValue ||
        !equalKey || !equalValue || !transformIntoNumber ||
        hashNumber <= 0) {
        return NULL; // Return NULL if validation fails.
    }

    // Allocate memory for the MultiHashTable structure.
    hashTableProMax table = (hashTableProMax)malloc(sizeof(*table));
    if (!table) { // Check if memory allocation failed.
        return NULL;
    }

    // Create the internal hash table using the provided function pointers and hash number.
    table->hashTable = createHashTable(
        copyKey,            // Function to copy keys.
        freeKey,            // Function to free keys.
        printKey,           // Function to print keys.
        copyListAsVoid,     // Function to copy values (as void pointers).
        destroyListWrapper, // Function to destroy values (as void pointers).
        printListAsVoid,    // Function to print values (as void pointers).
        equalKey,           // Function to compare keys.
        transformIntoNumber,// Function to hash keys.
        hashNumber          // Number of hash buckets.
    );

    // Check if the internal hash table creation failed.
    if (!table->hashTable) {
        free(table); // Free the allocated memory for the table.
        return NULL; // Return NULL to indicate failure.
    }

    // Initialize the fields of the MultiHashTable with the provided function pointers.
    table->copy_key = copyKey;
    table->free_key = freeKey;
    table->print_key = printKey;
    table->copy_value = copyValue;
    table->free_value = freeValue;
    table->print_value = printValue;
    table->equal_key = equalKey;
    table->equal_value = equalValue;
    table->transformIntoNumber = transformIntoNumber;

    // Return the pointer to the newly created MultiHashTable.
    return table;
}


status destroyHashTableProMax(hashTableProMax table) {
    // Check if the input 'table' pointer is NULL.
    // If it is NULL, there is nothing to destroy, so return 'failure'.
    if (!table) {
        return failure; // Invalid input, destruction cannot proceed.
    }

    // Destroy the internal hash table.
    // This operation is expected to handle:
    // - Destroying each bucket's linked list.
    // - Calling the appropriate cleanup function (e.g., freePairWithValueList)
    //   for each key-value pair stored in the hash table.
    destroyHashTable(table->hashTable);

    // Free the memory allocated for the MultiHashTable structure itself.
    free(table);

    // Return 'success' to indicate the MultiHashTable was successfully destroyed.
    return success;
}


/*
 * Returns the LinkedList of user-values associated with the given 'key',
 * or NULL if the key is not found in the hash table.
 */
Element lookupInHashTableProMax(hashTableProMax table, Element key) {
    // Check if the table or key is NULL.
    // If either is NULL, the lookup cannot proceed, so return NULL.
    if (!table || !key) {
        return NULL; // Invalid input.
    }

    // Use the internal hash table's lookup function to find the LinkedList
    // associated with the given key. If the key is not found, this will return NULL.
    LinkedList list = lookupInHashTable(table->hashTable, key);

    // Return the LinkedList of values associated with the key.
    // If the key does not exist in the table, NULL is returned.
    return list;
}


/*
 * addToHashTableProMax:
 * Adds a value to the MultiHashTable for the given key.
 * - If the key does not exist, creates a new KeyValuePair:
 *     * Copies the key (keyCopy).
 *     * Creates a new LinkedList to hold values.
 *     * Uses doNothingFree as 'free_value' for the LinkedList to avoid double-free issues.
 *     * Adds the new value to the list.
 * - If the key exists, appends the new value to the existing LinkedList.
 *
 * Parameters:
 * - table: Pointer to the MultiHashTable.
 * - key: The key to associate the value with.
 * - value: The value to add.
 *
 * Returns:
 * - success: If the value is added successfully.
 * - failure: If an error occurs (e.g., memory allocation failure).
 */
status addToHashTableProMax(hashTableProMax table, Element key, Element value) {
    // Validate input parameters.
    // Ensure that the table, key, and value are not NULL.
    if (!table || !key || !value) {
        return failure; // Invalid input.
    }

    // Check if the key already exists in the hash table.
    LinkedList existingValList = (LinkedList)lookupInHashTable(table->hashTable, key);

    // If the key exists, append the new value to the associated LinkedList.
    if (existingValList) {
        // Create a copy of the value using the provided copy function.
        Element valueCopy = table->copy_value(value);
        if (!valueCopy) { // Check for memory allocation failure.
            return failure;
        }

        // Append the copied value to the existing LinkedList.
        if (appendNode(existingValList, valueCopy) == failure) {
            table->free_value(valueCopy); // Free the value copy on failure.
            return failure;
        }

        return success; // Value added successfully.
    }
    // If the key does not exist, create a new LinkedList and add the value.
    else {
        // Create a new LinkedList to hold the values associated with the key.
        existingValList = createLinkedList(table->free_value, table->equal_value, table->print_value);
        if (!existingValList) { // Check for memory allocation failure.
            return failure;
        }

        // Create a copy of the value to add to the LinkedList.
        Element valueCopy = table->copy_value(value);
        if (!valueCopy) { // Check for memory allocation failure.
            destroyLinkedList(existingValList); // Free the LinkedList on failure.
            return failure;
        }

        // Append the copied value to the new LinkedList.
        if (appendNode(existingValList, valueCopy) == failure) {
            table->free_value(valueCopy); // Free the value copy on failure.
            destroyLinkedList(existingValList); // Free the LinkedList on failure.
            return failure;
        }

        // Add the new KeyValuePair (key and LinkedList) to the hash table.
        if (addToHashTable(table->hashTable, key, existingValList) == failure) {
            destroyLinkedList(existingValList); // Free the LinkedList on failure.
            return failure;
        }

        return success; // Key-value pair added successfully.
    }
}


/*
 * removeFromHashTableProMax:
 * Removes a key-value pair or a single value from the MultiHashTable.
 * - If `value == NULL`, the entire KeyValuePair (key and associated values) is removed.
 * - If `value != NULL`, the specific value is removed from the LinkedList associated with the key.
 *   If the LinkedList becomes empty after the removal, the key is removed entirely.
 *
 * Parameters:
 * - table: Pointer to the MultiHashTable.
 * - key: The key whose associated value(s) will be removed.
 * - value: The specific value to remove, or NULL to remove the entire KeyValuePair.
 *
 * Returns:
 * - success: If the key or value was removed successfully.
 * - failure: If an error occurs (e.g., invalid input, key not found, etc.).
 */
status removeFromHashTableProMax(hashTableProMax table, Element key, Element value) {
    // Validate input parameters.
    // Ensure that the table and key are not NULL.
    if (!table || !key) {
        return failure; // Invalid input.
    }

    // Lookup the LinkedList associated with the given key in the hash table.
    LinkedList existingValList = (LinkedList)lookupInHashTable(table->hashTable, key);

    // If the key exists in the hash table:
    if (existingValList) {
        // If a specific value is provided, attempt to remove it from the LinkedList.
        if (value) {
            // Remove the value from the LinkedList.
            if (deleteNode(existingValList, value) == failure) {
                return failure; // Failed to remove the value.
            }

            // If the LinkedList becomes empty after the removal, remove the key entirely.
            if (getLength(existingValList) == 0) {
                // Remove the key from the hash table.
                if (removeFromHashTable(table->hashTable, key) == failure) {
                    return failure; // Failed to remove the key.
                }
            }
        }
        // If value == NULL, remove the entire KeyValuePair (key and LinkedList).
        else {
            if (removeFromHashTable(table->hashTable, key) == failure) {
                return failure; // Failed to remove the KeyValuePair.
            }
        }
    }
    // If the key does not exist in the hash table, return failure.
    else {
        return failure;
    }

    // Return success if the operation completes without errors.
    return success;
}


/*
 * displayHashTableProMaxElementsByKey:
 * Displays the key and its associated list of values in the MultiHashTable.
 * - First, finds the sub-list of values associated with the given key.
 * - Prints the key, followed by the values in the sub-list.
 *
 * Parameters:
 * - table: Pointer to the MultiHashTable.
 * - key: The key whose associated values will be displayed.
 *
 * Returns:
 * - success: If the key and its values were successfully displayed.
 * - failure: If an error occurs (e.g., invalid input, key not found).
 */
status displayHashTableProMaxElementsByKey(hashTableProMax table, Element key) {
    // Validate input parameters.
    // Ensure that the table and key are not NULL.
    if (!table || !key) {
        return failure; // Invalid input.
    }

    // Retrieve the LinkedList of values associated with the given key.
    LinkedList valList = (LinkedList)lookupInHashTableProMax(table, key);

    // If the key is not found in the hash table, return failure.
    if (!valList) {
        return failure; // Key not found.
    }

    // Print the key using the provided print_key function.
    table->print_key(key);
    printf(" :\n"); // Print a separator for better readability.

    // Display the list of values associated with the key.
    displayList(valList);

    // Return success after displaying the key and its values.
    return success;
}
