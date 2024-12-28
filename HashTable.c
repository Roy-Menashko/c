#include <stdio.h>
#include <stdlib.h>
#include "HashTable.h"

struct hashTable_s {
    int size;
    FreeFunction free_key;
    FreeFunction free_value;
    PrintFunction print_key;
    PrintFunction print_value;
    CopyFunction copy_key;
    CopyFunction copy_value;
    EqualFunction equal_key;
    EqualFunction equal_value;
    TransformIntoNumberFunction transformIntoNumber;
    LinkedList* buckets;
};

static status freeKeyValuePairWrapper(Element e) {
    return destroyKeyValuePair((KeyValuePair)e);
}

static bool compareKeyValuePairWrapper(Element e1, Element e2) {
    KeyValuePair p1 = (KeyValuePair)e1;
    KeyValuePair p2 = (KeyValuePair)e2;
    return isEqualKey(p1, getKey(p2));
}

static status printKeyValuePairWrapper(Element e) {
    return displayKeyValue((KeyValuePair)e);
}

hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey,
                          CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue,
                          EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber,
                          int hashNumber) {
    if (!copyKey || !freeKey || !printKey || !copyValue || !freeValue || !printValue ||
        !equalKey || !transformIntoNumber) {
        return NULL;
    }
    hashTable newTable = (hashTable)malloc(sizeof(*newTable));
    if (!newTable) {
        return NULL;
    }
    newTable->size = hashNumber;
    newTable->copy_key = copyKey;
    newTable->free_key = freeKey;
    newTable->print_key = printKey;
    newTable->copy_value = copyValue;
    newTable->free_value = freeValue;
    newTable->print_value = printValue;
    newTable->equal_key = equalKey;
    newTable->equal_value = NULL;
    newTable->transformIntoNumber = transformIntoNumber;
    newTable->buckets = (LinkedList*)malloc(hashNumber * sizeof(LinkedList));
    if (!newTable->buckets) {
        free(newTable);
        return NULL;
    }
    for (int i = 0; i < hashNumber; i++) {
        newTable->buckets[i] = createLinkedList(freeKeyValuePairWrapper, compareKeyValuePairWrapper, printKeyValuePairWrapper);
        if (!newTable->buckets[i]) {
            destroyHashTable(newTable);
            return NULL;
        }
    }
    return newTable;
}

status destroyHashTable(hashTable table) {
    if (table == NULL) {
        return failure;
    }
    for (int i = 0; i < table->size; i++) {
        destroyLinkedList(table->buckets[i]);
    }
    free(table->buckets);
    free(table);
    table = NULL;
    return success;
}

status addToHashTable(hashTable table, Element key, Element value) {
    if (!table || !key || !value) {
        return failure;
    }

    /* נבצע העתקות (באמצעות table->copy_key ו-table->copy_value) */
    Element keyCopy = table->copy_key(key);
    if (!keyCopy) {
        return failure;
    }
    Element valueCopy = table->copy_value(value);
    if (!valueCopy) {
        table->free_key(keyCopy);
        return failure;
    }

    KeyValuePair pair = createKeyValuePair(
        keyCopy,
        valueCopy,
        table->free_key,
        table->free_value,
        table->print_key,
        table->print_value,
        table->equal_key
    );
    if (!pair) {
        /* אם נכשלנו, נשחרר את מה שהספקנו להקצות */
        table->free_key(keyCopy);
        table->free_value(valueCopy);
        return failure;
    }

    /* כעת נקבל אינדקס בטבלה על סמך keyCopy (אם רוצים על סמך key המקורי –
       זה לא משנה כל עוד פונקציית ההאש דטרמיניסטית עבור שני המצביעים) */
    int hash = table->transformIntoNumber(keyCopy) % table->size;
    if (appendNode(table->buckets[hash], pair) == failure) {
        destroyKeyValuePair(pair);
        return failure;
    }

    return success;
}

Element lookupInHashTable(hashTable table, Element key) {
    if (!table || !key) {
        return NULL;
    }
    int hash = (table->transformIntoNumber(key) + table->size) % table->size;
    LinkedList bucket = table->buckets[hash];
    if (!bucket) {
        return NULL;
    }
    Element e = getFirstElement(bucket);
    while (e) {
        KeyValuePair p = (KeyValuePair)e;
        if (table->equal_key(getKey(p), key)) {
            return getValue(p);
        }
        e = getNextElement(bucket, e);
    }
    return NULL;
}

status removeFromHashTable(hashTable table, Element key) {
    if (!table || !key) {
        return failure;
    }
    int hash = (table->transformIntoNumber(key) + table->size) % table->size;
    LinkedList bucket = table->buckets[hash];
    if (!bucket) {
        return failure;
    }
    Element e = getFirstElement(bucket);
    while (e) {
        KeyValuePair p = (KeyValuePair)e;
        if (table->equal_key(getKey(p), key)) {
            if (deleteNode(bucket, e) == failure) {
                return failure;
            }
            return success;
        }
        e = getNextElement(bucket, e);
    }
    return failure;
}

status displayHashElements(hashTable table) {
    if (!table) {
        return failure;
    }
    for (int i = 0; i < table->size; i++) {
        displayList(table->buckets[i]);
    }
    return success;
}

