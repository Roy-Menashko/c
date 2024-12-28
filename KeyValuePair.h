#ifndef KEY_VALUE_PAIR_H
#define KEY_VALUE_PAIR_H

#include <stdbool.h>
#include "Defs.h"



typedef struct KeyAndValuePair *KeyValuePair;

KeyValuePair createKeyValuePair(Element key, Element value,
                                 FreeFunction free_key, FreeFunction free_value,
                                 PrintFunction print_key, PrintFunction print_value,
                                 EqualFunction compare_key);

status destroyKeyValuePair(KeyValuePair pair);
status displayKey(KeyValuePair pair);
status displayValue(KeyValuePair pair);
status displayKeyValue(KeyValuePair pair);
Element getKey(KeyValuePair pair);
Element getValue(KeyValuePair pair);
bool isEqualKey(KeyValuePair pair, Element key);

#endif // KEY_VALUE_PAIR_H
