//
// Created by Avishur on 12/20/2024.
//
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "LinkedList.h"

struct Node {
    Element element;
    struct Node* next;
};

struct Linked_List {
    node tail;
    node head;
    int size;
    FreeFunction free_Func;
    EqualFunction compare_func;
    PrintFunction print_func;
};

LinkedList createLinkedList(FreeFunction free_func, EqualFunction compare_func, PrintFunction print_func) {
    LinkedList list = (LinkedList)malloc(sizeof(struct Linked_List));
    if (!list) {
        perror("Failed to allocate memory for LinkedList");
        return NULL;
    }

    list->free_Func = free_func;
    list->compare_func = compare_func;
    list->print_func = print_func;
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

status destroyLinkedList(LinkedList list) {
    if (list == NULL || list->free_Func == NULL) {
        return failure;
    }

    node current = list->head;
    while (current != NULL) {
        node next = current->next;
        list->free_Func(current->element);
        free(current);
        current = next;
    }

    free(list);
    return success;
}

status appendNode(LinkedList list, Element element) {
    if (list == NULL || list->free_Func == NULL || list->compare_func == NULL) {
        return failure;
    }

    node new_node = (node)malloc(sizeof(struct Node));
    if (new_node == NULL) {
        return failure;
    }

    new_node->element = element;
    new_node->next = NULL;

    if (list->size == 0) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->size++;
    return success;
}

status deleteNode(LinkedList list, Element element) {
    if (list == NULL || list->free_Func == NULL || list->compare_func == NULL || list->size == 0) {
        return failure;
    }

    node current = list->head;
    node previous = NULL;

    while (current != NULL) {
        if (list->compare_func(element, current->element)) {
            if (previous == NULL) {
                list->head = current->next;
            } else {
                previous->next = current->next;
            }

            if (current == list->tail) {
                list->tail = previous;
            }

            list->free_Func(current->element);
            free(current);
            list->size--;
            return success;
        }

        previous = current;
        current = current->next;
    }

    return failure;
}

status displayList(LinkedList list) {
    if (list == NULL || list->print_func == NULL) {
        return failure;
    }

    node current = list->head;
    while (current != NULL) {
        list->print_func(current->element);
        current = current->next;
    }
    printf("\n");
    return success;
}

Element getDataByIndex(LinkedList list, int index) {
    if (list == NULL || index < 0 || index >= list->size) {
        return NULL;
    }

    node current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    return current->element;
}

int getLength(LinkedList list) {
    if (list == NULL) {
        return -1;
    }
    return list->size;
}

Element searchByKeyInList(LinkedList list, Element dataKey, bool (*has_data)(Element element, Element dataKey)) {
    if (list == NULL || has_data == NULL) {
        return NULL;
    }

    node current = list->head;
    while (current != NULL) {
        if (has_data(current->element, dataKey)) {
            return current->element;
        }
        current = current->next;
    }
    return NULL;
}

Element getFirstElement(LinkedList list) {
    if (!list || !list->head) {
        return NULL;
    }
    return list->head->element;
}

Element getNextElement(LinkedList list, Element currentElement) {
    if (!list || !currentElement) {
        return NULL;
    }
    node temp = list->head;
    while (temp) {
        if (temp->element == currentElement) {
            // אם הבא קיים, נחזיר את ה-element שלו. אחרת NULL
            return temp->next ? temp->next->element : NULL;
        }
        temp = temp->next;
    }
    return NULL;
}