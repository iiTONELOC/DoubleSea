#pragma once
#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <stdio.h>  // For printf
#include <stdlib.h> // For malloc, free
#include "ListUtils.h"
#ifndef NULL
#define NULL ((void *)0)
#endif

typedef struct DoublyLinkedList DoublyLinkedList;
typedef struct DoublyLinkedNode DoublyLinkedNode;
typedef DoublyLinkedList DynamicDoublyLinkedList;
typedef DoublyLinkedNode DynamicDoublyLinkedNode;
typedef int (*OrderFunction)(void *pNode1, void *pNode2);
#define DOUBLY_LINKED_NODE_OFFSET offsetof(DoublyLinkedNode, pNext)
// __________________________ Structures __________________________
/**
 * @struct DoublyLinkedNode
 * @brief A node in a doubly linked list.
 *
 * This structure represents a node in a doubly linked list, containing pointers
 * to the next and previous nodes, as well as a pointer to the data it holds.
 *
 * This is a generic structure, so the data pointer can be cast to any type.
 *
 * ```c
 * typedef struct DoublyLinkedNode
 * {
 *    void *pData;                           // Pointer to the data the node holds
 *    void *pPrev;                           // Pointer to the previous node in the list
 *    void *pNext;                           // Pointer to the next node in the list
 *    int dynamic;                           // Flag to indicate if the node was dynamically allocated
 * } DoublyLinkedNode;
 * ```
 */
struct DoublyLinkedNode
{
    void *pData;
    void *pNext;
    void *pPrev; // should always follow pNext in memory
    int dynamic;
};

/**
 * @struct DoublyLinkedList
 * @brief A doubly linked list.
 *
 * This structure represents a doubly linked list, containing pointers to the head
 * and tail of the list, as well as the number of nodes, and a pointer to a function
 * used to order nodes on insertion.
 *
 * This is a generic structure, the data pointers can be cast to any type. This allows
 * the list to use any data type that has the previous pointer following the next
 * pointer in memory. The offset field is used to determine the offset to the pNext
 * field in the data structure, we can then increase the pointer to the data by the
 * size of the void pointer to get the pNext
 *
 * ```c
 * typedef struct DoublyLinkedList
 * {
 *   void *pHead;                                       // Pointer to the head of the list
 *   void *pTail;                                       // Pointer to the tail of the list
 *   size_t count;                                      // Number of nodes in the list
 *   int dynamic;                                       // Dynamic allocation ?
 *   int (*OrderFunction)(void *pNode1, void *pNode2);  // Pointer to an OrderFunction
 * } DoublyLinkedList;
 * ```
 */
struct DoublyLinkedList
{
    void *pHead;                 // Pointer to the head of the list
    void *pTail;                 // Pointer to the tail of the list
    int dynamic;                 // Dynamic allocation ?
    size_t count;                // Number of nodes in the list
    size_t offset;               // Offset to the pNext field in the data structure
    OrderFunction orderFunction; // Pointer to an OrderFunction
};

// __________________________ Function Prototypes __________________________

void *Pop(DoublyLinkedList *pFromList);
void Push(void *pNode, DoublyLinkedList *pIntoList);
void DestroyDoublyLinkedNode(DoublyLinkedNode *pNode);
void RemoveNode(void *pNode, DoublyLinkedList *pFromList);
void InsertNode(void *pNode, DoublyLinkedList *pIntoList);
DynamicDoublyLinkedNode *CreateDoublyLinkedNode(void *pData);
void DestroyDoublyLinkedList(DoublyLinkedList *pList, int cleanNodes);
void InitializeDoublyLinkedNode(int isDynamic, DoublyLinkedNode *pNode, void *pWithData);
DynamicDoublyLinkedList *CreateDoublyLinkedList(size_t offset, OrderFunction pOrderFunction);
void InitializeDoublyLinkedList(int isDynamic, size_t offset, DoublyLinkedList *pList, OrderFunction pOrderFunction);
#endif
