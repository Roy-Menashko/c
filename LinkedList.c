//
// Created by Avishur on 12/20/2024.
//
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "LinkedList.h"

// Definition of the Node structure
struct Node {
    Element element;       // The element/data stored in the node
    struct Node* next;     // Pointer to the next node in the linked list
};

// Definition of the Linked_List structure
struct Linked_List {
    node tail;             // Pointer to the last node in the linked list
    node head;             // Pointer to the first node in the linked list
    int size;              // The number of elements in the linked list
    FreeFunction free_Func;  // Function pointer for freeing the memory of elements
    EqualFunction compare_func; // Function pointer for comparing two elements
    PrintFunction print_func;   // Function pointer for printing an element
};


LinkedList createLinkedList(FreeFunction free_func, EqualFunction compare_func, PrintFunction print_func) {
    // Allocate memory for a new Linked_List structure
    LinkedList list = (LinkedList)malloc(sizeof(struct Linked_List));
    if (!list) { // Check if memory allocation failed
        perror("Failed to allocate memory for LinkedList"); // Print an error message
        return NULL; // Return NULL if memory allocation failed
    }

    // Initialize the function pointers
    list->free_Func = free_func;        // Set the function for freeing elements
    list->compare_func = compare_func; // Set the function for comparing elements
    list->print_func = print_func;     // Set the function for printing elements

    // Initialize the list's properties
    list->size = 0;   // Set the size of the list to 0 (empty list)
    list->head = NULL; // Set the head pointer to NULL (no nodes yet)
    list->tail = NULL; // Set the tail pointer to NULL (no nodes yet)

    return list; // Return the newly created and initialized linked list
}


status destroyLinkedList(LinkedList list) {
    // Check if the list or the free function is NULL
    if (list == NULL || list->free_Func == NULL) {
        return failure; // Return failure if the list or free function is not initialized
    }

    node current = list->head; // Start with the head of the linked list
    while (current != NULL) { // Loop through all nodes in the list
        node next = current->next; // Save the pointer to the next node
        list->free_Func(current->element); // Free the memory of the element stored in the current node
        free(current); // Free the current node
        current = next; // Move to the next node
    }

    free(list); // Free the memory allocated for the linked list structure itself
    return success; // Return success to indicate the list was successfully destroyed
}


status appendNode(LinkedList list, Element element) {
    // Check if the list or its required functions are NULL
    if (list == NULL || list->free_Func == NULL || list->compare_func == NULL) {
        return failure; // Return failure if the list or functions are not properly initialized
    }

    // Allocate memory for a new node
    node new_node = (node)malloc(sizeof(struct Node));
    if (new_node == NULL) { // Check if memory allocation failed
        return failure; // Return failure if the node could not be created
    }

    // Initialize the new node with the given element and set its next pointer to NULL
    new_node->element = element;
    new_node->next = NULL;

    // If the list is empty, set the new node as both the head and the tail
    if (list->size == 0) {
        list->head = new_node; // Set the head to the new node
        list->tail = new_node; // Set the tail to the new node
    } else {
        // If the list is not empty, add the new node to the end
        list->tail->next = new_node; // Link the current tail to the new node
        list->tail = new_node; // Update the tail to the new node
    }

    // Increment the size of the list
    list->size++;

    return success; // Return success to indicate the node was appended
}


status deleteNode(LinkedList list, Element element) {
    // Check if the list, its required functions, or the size are invalid
    if (list == NULL || list->free_Func == NULL || list->compare_func == NULL || list->size == 0) {
        return failure; // Return failure if the list is not properly initialized or empty
    }

    node current = list->head; // Start with the head of the list
    node previous = NULL;      // Pointer to keep track of the previous node (initialized to NULL)

    // Traverse the linked list
    while (current != NULL) {
        // Check if the current node contains the target element
        if (list->compare_func(element, current->element)) {
            // If the node to be deleted is the head
            if (previous == NULL) {
                list->head = current->next; // Update the head to the next node
            } else {
                previous->next = current->next; // Bypass the current node
            }

            // If the node to be deleted is the tail
            if (current == list->tail) {
                list->tail = previous; // Update the tail to the previous node
            }

            // Free the memory of the element and the node itself
            list->free_Func(current->element);
            free(current);

            // Decrease the size of the list
            list->size--;

            return success; // Return success after deleting the node
        }

        // Move to the next node
        previous = current;
        current = current->next;
    }

    // If the element was not found, return failure
    return failure;
}


status displayList(LinkedList list) {
    // Check if the list or the print function is NULL
    if (list == NULL || list->print_func == NULL) {
        return failure; // Return failure if the list or print function is not initialized
    }

    node current = list->head; // Start with the head of the list

    // Traverse the linked list and print each element
    while (current != NULL) {
        list->print_func(current->element); // Call the user-defined print function for the current element
        current = current->next; // Move to the next node in the list
    }

    return success; // Return success after printing all elements
}

Element getDataByIndex(LinkedList list, int index) {
    // Validate the list and the index
    if (list == NULL || index < 1 || index >= list->size) {
        return NULL; // Return NULL if the list is not initialized or the index is out of bounds
    }

    node current = list->head; // Start at the head of the list

    // Traverse the list to the specified index
    for (int i = 1; i < index; i++) {
        current = current->next; // Move to the next node
    }

    return current->element; // Return the element at the specified index
}


int getLength(LinkedList list) {
    // Check if the list is NULL
    if (list == NULL) {
        return -1; // Return -1 to indicate that the list is invalid or not initialized
    }
    return list->size; // Return the number of elements (size) in the list
}


Element searchByKeyInList(LinkedList list, Element dataKey, bool (*has_data)(Element element, Element dataKey)) {
    // Check if the list or the has_data function is NULL
    if (list == NULL || has_data == NULL) {
        return NULL; // Return NULL if the list or comparison function is not initialized
    }

    node current = list->head; // Start at the head of the list

    // Traverse the linked list
    while (current != NULL) {
        // Check if the current element matches the key using the has_data function
        if (has_data(current->element, dataKey)) {
            return current->element; // Return the element if a match is found
        }
        current = current->next; // Move to the next node
    }

    return NULL; // Return NULL if no matching element is found
}


Element getFirstElement(LinkedList list) {
    // Check if the list or the head of the list is NULL
    if (!list || !list->head) {
        return NULL; // Return NULL if the list is not initialized or empty
    }

    return list->head->element; // Return the element stored in the head node
}


Element getNextElement(LinkedList list, Element currentElement) {
    // Check if the list or the current element is NULL
    if (!list || !currentElement) {
        return NULL; // Return NULL if the list is not initialized or the current element is invalid
    }

    node temp = list->head; // Start at the head of the list

    // Traverse the list to find the node containing the current element
    while (temp) {
        if (temp->element == currentElement) {
            // If the next node exists, return its element; otherwise, return NULL
            return temp->next ? temp->next->element : NULL;
        }
        temp = temp->next; // Move to the next node
    }

    return NULL; // Return NULL if the current element is not found in the list
}


Element get_data(node node) {
    // Check if the node is NULL
    if (!node) {
        return NULL; // Return NULL if the node is invalid
    }

    return node->element; // Return the element stored in the node
}
