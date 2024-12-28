//
// Created by Avishur on 12/22/2024.
//

#ifndef MULTIVALUEHASHTABLE_H
#define MULTIVALUEHASHTABLE_H
#include <stdbool.h>
#include "Defs.h"
#include "HashTable.h"

typedef struct MultiHashTable *hashTableProMax;

hashTableProMax createHashTableProMax(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,EqualFunction equal_value, TransformIntoNumberFunction transformIntoNumber, int hashNumber);
status destroyHashTableProMax(hashTableProMax table);
status addToHashTableProMax(hashTableProMax hashTableProMax,Element key,Element value);
Element lookupInHashTableProMax(hashTableProMax hashTableProMax,Element key);
status removeFromHashTableProMax(hashTableProMax hashTableProMax,Element key,Element value);
status displayHashTableProMaxElementsByKey(hashTableProMax hashTableProMax , Element key);

#endif //MULTIVALUEHASHTABLE_H
