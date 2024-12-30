
#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <stdbool.h>
#include "Defs.h"


typedef struct hashTable_s *hashTable;

hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber, int hashNumber);
status destroyHashTable(hashTable table);
status addToHashTable(hashTable table, Element key,Element value);
Element lookupInHashTable(hashTable table, Element key);
status removeFromHashTable(hashTable table, Element key);
status displayHashElements(hashTable table);

#endif /* HASH_TABLE_H */
