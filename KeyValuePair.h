#ifndef KEY_VALUE_PAIR_H
#define KEY_VALUE_PAIR_H


#include "Defs.h"



// Define KeyValuePair as a pointer to a KeyAndValuePair structure
typedef struct KeyAndValuePair *KeyValuePair;

// Function to create a KeyValuePair structure
// key: The key element of the pair
// value: The value element of the pair
// free_key: Function to free the key
// free_value: Function to free the value
// print_key: Function to print the key
// print_value: Function to print the value
// compare_key: Function to compare keys
KeyValuePair createKeyValuePair(Element key, Element value,
                                 FreeFunction free_key, FreeFunction free_value,
                                 PrintFunction print_key, PrintFunction print_value,
                                 EqualFunction compare_key);

// Function to destroy a KeyValuePair structure
// pair: The KeyValuePair to be destroyed
status destroyKeyValuePair(KeyValuePair pair);

// Function to display the key of a KeyValuePair
// pair: The KeyValuePair whose key is to be displayed
status displayKey(KeyValuePair pair);

// Function to display the value of a KeyValuePair
// pair: The KeyValuePair whose value is to be displayed
status displayValue(KeyValuePair pair);

// Function to display both the key and value of a KeyValuePair
// pair: The KeyValuePair whose key and value are to be displayed
status displayKeyValue(KeyValuePair pair);

// Function to retrieve the key from a KeyValuePair
// pair: The KeyValuePair whose key is to be retrieved
// Returns: The key element of the pair
Element getKey(KeyValuePair pair);

// Function to retrieve the value from a KeyValuePair
// pair: The KeyValuePair whose value is to be retrieved
// Returns: The value element of the pair
Element getValue(KeyValuePair pair);

// Function to check if a given key is equal to the key in a KeyValuePair
// pair: The KeyValuePair to check against
// key: The key to compare
// Returns: true if the keys are equal, false otherwise
bool isEqualKey(KeyValuePair pair, Element key);


#endif // KEY_VALUE_PAIR_H
