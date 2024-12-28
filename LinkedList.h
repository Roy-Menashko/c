//
// Created by Avishur on 12/20/2024.
//

#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdbool.h>
#include "Defs.h"





typedef struct Linked_List *LinkedList;
typedef struct Node* node;

LinkedList createLinkedList(FreeFunction free_func,EqualFunction compare_func,PrintFunction print_func);

status destroyLinkedList(LinkedList list);

status appendNode(LinkedList list,Element);

status deleteNode(LinkedList list,Element);

status displayList(LinkedList list);

Element getDataByIndex(LinkedList list, int index);

int getLength(LinkedList list);

Element searchByKeyInList(LinkedList list,Element element, bool (*has_data)(Element element, Element dataKey));

Element getFirstElement(LinkedList list);

Element getNextElement(LinkedList list, Element currentElement);

#endif //LINKEDLIST_H
