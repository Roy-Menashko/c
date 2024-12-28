#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MultiValueHashTable.h"



/*
   This multi-value hash table:
     - Each bucket is a LinkedList of KeyValuePair.
     - The KeyValuePair's "value" is itself a LinkedList of user-provided values.
*/

/*
   We define a "dummy" free function for the sub-LinkedList pointer,
   so destroyKeyValuePair won't double-free it.
*/
static status doNothingFree(Element e) {
    (void)e;
    return success; /* No-op */
}

/*
   When removing a KeyValuePair from the bucket,
   we must FIRST destroy the sub-LinkedList of values,
   THEN call destroyKeyValuePair.
*/
static status freePairWithValueList(Element e) {
    KeyValuePair pair = (KeyValuePair)e;

    /* Destroy the sub-list of user-values. */
    destroyLinkedList((LinkedList)getValue(pair));

    /* Now destroy the pair itself:
       Because we gave the pair a "doNothingFree" as free_value,
       destroyKeyValuePair() won't double-free the pointer. */
    return destroyKeyValuePair(pair);
}

/* We compare KeyValuePairs by key. */
static bool comparePairByKey(Element e1, Element e2) {
    KeyValuePair p1 = (KeyValuePair)e1;
    KeyValuePair p2 = (KeyValuePair)e2;
    return isEqualKey(p1, getKey(p2));
}

/* If we ever want to display each KeyValuePair in a bucket, we print key + values. */
static status printPairKeyThenValueList(Element e) {
    KeyValuePair pair = (KeyValuePair)e;
    displayKey(pair);
    displayList((LinkedList)getValue(pair));
    return success;
}

struct MultiHashTable {
    int size;

    CopyFunction copy_key;
    FreeFunction free_key;
    PrintFunction print_key;

    CopyFunction copy_value;
    FreeFunction free_value;
    PrintFunction print_value;

    EqualFunction equal_key;
    EqualFunction equal_value;

    TransformIntoNumberFunction transformIntoNumber;

    LinkedList* buckets; /* An array of LinkedList of KeyValuePair. */
};

/* Simple helper to compute the bucket index. */
static int getBucketIndex(hashTableProMax table, Element key) {
    int hash = table->transformIntoNumber(key);
    if (hash < 0) {
        hash = -hash;
    }
    return hash % table->size;
}

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
    if (!copyKey || !freeKey || !printKey ||
        !copyValue || !freeValue || !printValue ||
        !equalKey || !equalValue || !transformIntoNumber ||
        hashNumber <= 0) {
        return NULL;
    }

    hashTableProMax table = (hashTableProMax)malloc(sizeof(*table));
    if (!table) {
        return NULL;
    }

    table->size = hashNumber;
    table->copy_key = copyKey;
    table->free_key = freeKey;
    table->print_key = printKey;
    table->copy_value = copyValue;
    table->free_value = freeValue;
    table->print_value = printValue;
    table->equal_key = equalKey;
    table->equal_value = equalValue;
    table->transformIntoNumber = transformIntoNumber;

    table->buckets = (LinkedList*)malloc(hashNumber * sizeof(LinkedList));
    if (!table->buckets) {
        free(table);
        return NULL;
    }

    /* Each bucket is a LinkedList of KeyValuePair. */
    for (int i = 0; i < hashNumber; i++) {
        table->buckets[i] = createLinkedList(
            freePairWithValueList,
            comparePairByKey,
            printPairKeyThenValueList
        );
        if (!table->buckets[i]) {
            /* Clean up previously allocated buckets. */
            for (int j = 0; j < i; j++) {
                destroyLinkedList(table->buckets[j]);
            }
            free(table->buckets);
            free(table);
            return NULL;
        }
    }
    return table;
}

status destroyHashTableProMax(hashTableProMax table) {
    if (!table) {
        return failure;
    }
    /* Destroy each bucket's LinkedList, which calls freePairWithValueList. */
    for (int i = 0; i < table->size; i++) {
        destroyLinkedList(table->buckets[i]);
    }
    free(table->buckets);
    free(table);
    return success;
}

/* Returns the LinkedList of user-values for 'key', or NULL if not found. */
Element lookupInHashTableProMax(hashTableProMax table, Element key) {
    if (!table || !key) {
        return NULL;
    }
    int index = getBucketIndex(table, key);
    LinkedList bucket = table->buckets[index];

    Element nodeElem = getFirstElement(bucket);
    while (nodeElem) {
        KeyValuePair pair = (KeyValuePair)nodeElem;
        if (table->equal_key(getKey(pair), key)) {
            return getValue(pair); /* the sub-LinkedList of values */
        }
        nodeElem = getNextElement(bucket, nodeElem);
    }
    return NULL;
}

/*
   addToHashTableProMax:
   - If key not found, create a new KeyValuePair:
       * keyCopy
       * new LinkedList for the values
       * pass doNothingFree as 'free_value' to KeyValuePair so we don't double-free the sub-list
     Then append the new value to that list.
   - If key found, just append the new value to the existing sub-list.
*/
status addToHashTableProMax(hashTableProMax table, Element key, Element value) {
    if (!table || !key || !value) {
        return failure;
    }

    /* Does key exist? */
    LinkedList existingValList = (LinkedList)lookupInHashTableProMax(table, key);
    if (!existingValList) {
        /* Need a new KeyValuePair: (keyCopy) -> (newValList). */
        Element keyCopy = table->copy_key(key);
        if (!keyCopy) {
            return failure;
        }
        LinkedList newValList = createLinkedList(
            table->free_value,
            table->equal_value,
            table->print_value
        );
        if (!newValList) {
            table->free_key(keyCopy);
            return failure;
        }

        /* Create a KeyValuePair that won't free 'newValList' again. */
        KeyValuePair pair = createKeyValuePair(
            keyCopy,
            newValList,
            table->free_key,
            doNothingFree,  /* <--- crucial: skip freeing the sub-list pointer */
            table->print_key,
            table->print_value,
            table->equal_key
        );
        if (!pair) {
            table->free_key(keyCopy);
            destroyLinkedList(newValList);
            return failure;
        }

        int bucketIndex = getBucketIndex(table, key);
        if (appendNode(table->buckets[bucketIndex], pair) == failure) {
            destroyKeyValuePair(pair); /* which calls table->free_key on keyCopy, doNothingFree on valList pointer */
            return failure;
        }

        /* Also insert the first 'value' into the sub-list. */
        Element valCopy = table->copy_value(value);
        if (!valCopy) {
            /* remove the entire pair from the bucket. */
            deleteNode(table->buckets[bucketIndex], pair);
            destroyKeyValuePair(pair);
            return failure;
        }
        if (appendNode(newValList, valCopy) == failure) {
            table->free_value(valCopy);
            deleteNode(table->buckets[bucketIndex], pair);
            destroyKeyValuePair(pair);
            return failure;
        }
    } else {
        /* Key found -> just append the new value to existingValList. */
        Element valCopy = table->copy_value(value);
        if (!valCopy) {
            return failure;
        }
        if (appendNode(existingValList, valCopy) == failure) {
            table->free_value(valCopy);
            return failure;
        }
    }
    return success;
}

/*
   removeFromHashTableProMax:
   - If value == NULL, remove entire KeyValuePair (i.e. remove the key).
   - Else remove just that single value. If the sub-list becomes empty, remove the key entirely.
*/
status removeFromHashTableProMax(hashTableProMax table, Element key, Element value) {
    if (!table || !key) {
        return failure;
    }
    int index = getBucketIndex(table, key);
    LinkedList bucket = table->buckets[index];

    /* Find the KeyValuePair in the bucket. */
    Element nodeElem = getFirstElement(bucket);
    KeyValuePair foundPair = NULL;
    while (nodeElem) {
        KeyValuePair p = (KeyValuePair)nodeElem;
        if (table->equal_key(getKey(p), key)) {
            foundPair = p;
            break;
        }
        nodeElem = getNextElement(bucket, nodeElem);
    }
    if (!foundPair) {
        return failure; /* key not found */
    }

    if (!value) {
        /* Remove the entire key (which calls freePairWithValueList on foundPair). */
        if (deleteNode(bucket, foundPair) == failure) {
            return failure;
        }
        return success;
    }

    /* Otherwise, remove a single value from the sub-list. */
    LinkedList valList = (LinkedList)getValue(foundPair);
    if (!valList) {
        return failure;
    }
    if (deleteNode(valList, value) == failure) {
        return failure; /* that value wasn't in the list */
    }
    /* If the sub-list is now empty, remove the entire key. */
    if (getLength(valList) == 0) {
        if (deleteNode(bucket, foundPair) == failure) {
            return failure;
        }
    }
    return success;
}

/*
   displayHashTableProMaxElementsByKey:
   - We find the sub-list of values for 'key'.
   - Print the key, then print the sub-list of values.
*/
status displayHashTableProMaxElementsByKey(hashTableProMax table, Element key) {
    if (!table || !key) {
        return failure;
    }
    LinkedList valList = (LinkedList)lookupInHashTableProMax(table, key);
    if (!valList) {
        return failure; /* key not found */
    }
    table->print_key(key);
    displayList(valList);
    return success;
}