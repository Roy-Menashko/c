#include "KeyValuePair.h"
#include <stdlib.h>
#include <stdio.h>

struct KeyAndValuePair {
    Element key;                 // The key element of the pair
    Element value;               // The value element of the pair
    FreeFunction free_key;       // Function pointer to free the memory of the key
    FreeFunction free_value;     // Function pointer to free the memory of the value
    PrintFunction print_key;     // Function pointer to print the key
    PrintFunction print_value;   // Function pointer to print the value
    EqualFunction compare_key;   // Function pointer to compare keys
};


KeyValuePair createKeyValuePair(Element key, Element value,
                                FreeFunction free_key, FreeFunction free_value,
                                PrintFunction print_key, PrintFunction print_value,
                                EqualFunction compare_key)
{
    // Validate input parameters
    if (!key || !value || !free_key || !free_value || !print_key || !print_value || !compare_key) {
        // If any parameter is NULL, free allocated resources and return NULL
        if (key) {
            free_key(key); // Free the allocated key if it's valid
        }
        if (value) {
            free_value(value); // Free the allocated value if it's valid
        }
        return NULL; // Return NULL indicating failure
    }

    // Allocate memory for the KeyValuePair structure
    KeyValuePair pair = (KeyValuePair)malloc(sizeof(*pair));
    if (!pair) { // Check if memory allocation failed
        // If allocation fails, free the key and value to prevent memory leaks
        free_key(key);
        free_value(value);
        return NULL; // Return NULL indicating failure
    }

    // Initialize the fields of the KeyValuePair
    pair->key = key;                // Assign the key to the pair
    pair->value = value;            // Assign the value to the pair
    pair->free_key = free_key;      // Assign the key's free function
    pair->free_value = free_value;  // Assign the value's free function
    pair->print_key = print_key;    // Assign the key's print function
    pair->print_value = print_value;// Assign the value's print function
    pair->compare_key = compare_key;// Assign the key comparison function

    return pair; // Return the newly created KeyValuePair
}


status destroyKeyValuePair(KeyValuePair pair) {
    // Check if the KeyValuePair is NULL
    if (!pair) return failure; // Return failure if the pair is invalid

    // Free the key if the free_key function is provided
    if (pair->free_key) pair->free_key(pair->key);

    // Free the value if the free_value function is provided
    if (pair->free_value) pair->free_value(pair->value);

    // Free the memory allocated for the KeyValuePair structure itself
    free(pair);

    return success; // Return success to indicate the pair was destroyed successfully
}


status displayKey(KeyValuePair pair) {
    // Check if the KeyValuePair or the print_key function is NULL
    if (!pair || !pair->print_key) return failure; // Return failure if the pair or the print function is invalid

    // Call the print_key function to display the key
    pair->print_key(pair->key);

    return success; // Return success to indicate the key was displayed successfully
}


status displayValue(KeyValuePair pair) {
    // Check if the KeyValuePair or the print_value function is NULL
    if (!pair || !pair->print_value) return failure; // Return failure if the pair or the print function is invalid

    // Call the print_value function to display the value
    pair->print_value(pair->value);

    return success; // Return success to indicate the value was displayed successfully
}


status displayKeyValue(KeyValuePair pair) {
    // Check if the KeyValuePair or its print functions (key and value) are NULL
    if (!pair || !pair->print_key || !pair->print_value) return failure;
    // Return failure if the pair or either print function is invalid

    // Call the print_key function to display the key
    pair->print_key(pair->key);

    // Call the print_value function to display the value
    pair->print_value(pair->value);

    return success; // Return success to indicate both key and value were displayed successfully
}


Element getKey(KeyValuePair pair) {
    // Check if the KeyValuePair is valid
    // If the pair is valid, return its key; otherwise, return NULL
    return (pair) ? pair->key : NULL;
}


Element getValue(KeyValuePair pair) {
    // Check if the KeyValuePair is valid
    // If the pair is valid, return its value; otherwise, return NULL
    return (pair) ? pair->value : NULL;
}


bool isEqualKey(KeyValuePair pair, Element key) {
    if (!pair || !key || !pair->compare_key) return false;
    return pair->compare_key(pair->key, key);
}
