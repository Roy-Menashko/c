#include "KeyValuePair.h"
#include <stdlib.h>
#include <stdio.h>

struct KeyAndValuePair {
    Element key;
    Element value;
    FreeFunction free_key;
    FreeFunction free_value;
    PrintFunction print_key;
    PrintFunction print_value;
    EqualFunction compare_key;

};

KeyValuePair createKeyValuePair(Element key, Element value,
                                FreeFunction free_key, FreeFunction free_value,
                                PrintFunction print_key, PrintFunction print_value,
                                EqualFunction compare_key)
{
    if (!key || !value || !free_key || !free_value || !print_key || !print_value || !compare_key) {
        // FIX: לפני שחוזרים NULL, לשחרר מה שכבר הוקצה:
        if (key) {
            free_key(key);
        }
        if (value) {
            free_value(value);
        }
        return NULL;
    }

    KeyValuePair pair = (KeyValuePair)malloc(sizeof(*pair));
    if (!pair) {
        // FIX: אם pair == NULL, לשחרר key,value
        free_key(key);
        free_value(value);
        return NULL;
    }

    pair->key = key;
    pair->value = value;
    pair->free_key = free_key;
    pair->free_value = free_value;
    pair->print_key = print_key;
    pair->print_value = print_value;
    pair->compare_key = compare_key;

    return pair;
}

status destroyKeyValuePair(KeyValuePair pair) {
    if (!pair) return failure;

    if (pair->free_key) pair->free_key(pair->key);
    if (pair->free_value) pair->free_value(pair->value);
    free(pair);

    return success;
}

status displayKey(KeyValuePair pair) {
    if (!pair || !pair->print_key) return failure;
    pair->print_key(pair->key);
    return success;
}

status displayValue(KeyValuePair pair) {
    if (!pair || !pair->print_value) return failure;
    pair->print_value(pair->value);
    return success;
}

status displayKeyValue(KeyValuePair pair) {
    if (!pair || !pair->print_key || !pair->print_value) return failure;

    pair->print_key(pair->key);
    pair->print_value(pair->value);

    return success;
}

Element getKey(KeyValuePair pair) {
    return (pair) ? pair->key : NULL;
}

Element getValue(KeyValuePair pair) {
    return (pair) ? pair->value : NULL;
}

bool isEqualKey(KeyValuePair pair, Element key) {
    if (!pair || !key || !pair->compare_key) return false;
    return pair->compare_key(pair->key, key);
}
