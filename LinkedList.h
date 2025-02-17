//
// Created by Avishur on 12/20/2024.
//

#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Defs.h"





typedef struct Linked_List *LinkedList; // Defines LinkedList as a pointer to a Linked_List structure
typedef struct Node* node;             // Defines node as a pointer to a Node structure

// Function to create a linked list
// free_func: A function to free memory for elements in the list
// compare_func: A function to compare two elements
// print_func: A function to print an element
LinkedList createLinkedList(FreeFunction free_func, EqualFunction compare_func, PrintFunction print_func);

// Function to destroy a linked list and free all associated memory
// list: A pointer to the linked list
status destroyLinkedList(LinkedList list);

// Function to append a new node to the end of the linked list
// list: A pointer to the linked list
// Element: The element to be added
status appendNode(LinkedList list, Element);

// Function to delete a node from the linked list
// list: A pointer to the linked list
// Element: The element to be deleted
status deleteNode(LinkedList list, Element);

// Function to display all elements in the linked list
// list: A pointer to the linked list
status displayList(LinkedList list);

// Function to get the element at a specific index in the linked list
// list: A pointer to the linked list
// index: The index of the desired element
Element getDataByIndex(LinkedList list, int index);

// Function to get the number of nodes in the linked list
// list: A pointer to the linked list
int getLength(LinkedList list);

// Function to search for an element in the linked list by a specific key
// list: A pointer to the linked list
// element: The element to search for
// has_data: A function to check if the element matches the key
Element searchByKeyInList(LinkedList list, Element element, bool (*has_data)(Element element, Element dataKey));

// Function to get the first element in the linked list
// list: A pointer to the linked list
Element getFirstElement(LinkedList list);

// Function to get the next element in the linked list after a given element
// list: A pointer to the linked list
// currentElement: The current element
Element getNextElement(LinkedList list, Element currentElement);

// Function to get the data stored in a specific node
// node: A pointer to a node in the linked list
Element get_data(node node);

#endif //LINKEDLIST_H
