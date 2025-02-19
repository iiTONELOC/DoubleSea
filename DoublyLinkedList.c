#include "ListUtils.h"
#include "DoublyLinkedList.h"

/**
 * @brief Destroys a doubly linked node.
 *
 * This function destroys a doubly linked node
 * if it was dynamically allocated. Otherwise, it
 * resets the node's values.
 *
 * @param pNode Pointer to the DoublyLinkedNode structure.
 * @return Nothing
 */
void DestroyDoublyLinkedNode(DoublyLinkedNode *pNode)
{
    if (pNode == NULL)
    {
        return;
    }
    else if (pNode->dynamic == 1)
    {
        free(pNode);
    }
    else
    {
        InitializeDoublyLinkedNode(0, pNode, NULL);
    }
}

/**
 * @brief Removes a node from a doubly linked list.
 *
 * This function removes a node from a doubly linked list.
 *
 * @param pNode Pointer to the DoublyLinkedNode structure.
 * @param pFromList Pointer to the DoublyLinkedList structure.
 * @return Nothing
 */
void RemoveNode(void *pNode, DoublyLinkedList *pFromList)
{
    if (pFromList == NULL || pNode == NULL || pFromList->count == 0)
        return;

    // Get pointers to the next and previous nodes in the list
    void **pNodeNext = GetNextPointer(pNode, pFromList->offset);
    void **pNodePrev = GetPrevPointer(pNode, pFromList->offset);

    // If the node is the head of the list
    if (pFromList->pHead == pNode)
    {
        pFromList->pHead = *pNodeNext;
        if (pFromList->pHead) // If there's a new head, update its prev pointer
        {
            void **pHeadPrev = GetPrevPointer(pFromList->pHead, pFromList->offset);
            *pHeadPrev = NULL;
        }
    }

    // If the node is the tail of the list
    if (pFromList->pTail == pNode)
    {
        pFromList->pTail = *pNodePrev;
        if (pFromList->pTail) // If there's a new tail, update its next pointer
        {
            void **pTailNext = GetNextPointer(pFromList->pTail, pFromList->offset);
            *pTailNext = NULL;
        }
    }

    // If the node is in the middle of the list
    // No need to traverse, the list, just update the pointers
    // so that the previous node points to the next node and vice versa
    // this will remove the node from the list
    if (*pNodePrev != NULL)
    {
        void **pPrevNext = GetNextPointer(*pNodePrev, pFromList->offset);
        *pPrevNext = *pNodeNext;
    }

    if (*pNodeNext != NULL)
    {
        void **pNextPrev = GetPrevPointer(*pNodeNext, pFromList->offset);
        *pNextPrev = *pNodePrev;
    }

    // Reset the removed node's pointers to avoid dangling references
    *pNodeNext = NULL;
    *pNodePrev = NULL;

    // Decrement the count
    pFromList->count--;

    // If the list is empty now, reset head and tail
    if (pFromList->count == 0)
    {
        pFromList->pHead = NULL;
        pFromList->pTail = NULL;
    }
}

/**
 * @brief Inserts a node into a doubly linked list.
 *
 * This function inserts a node into a doubly linked list.
 *
 * @param pNode Pointer to the DoublyLinkedNode structure.
 * @param pIntoList Pointer to the DoublyLinkedList structure.
 * @return Nothing
 */
void InsertNode(void *pNode, DoublyLinkedList *pIntoList)
{
    if (pIntoList == NULL || pNode == NULL)
        return;

    // Get pointers to the next and previous nodes in the list
    void **pNodeNext = GetNextPointer(pNode, pIntoList->offset);
    void **pNodePrev = GetPrevPointer(pNode, pIntoList->offset);

    // If the list is empty, set head and tail to the new node
    if (pIntoList->count == 0)
    {
        pIntoList->pHead = pNode;
        pIntoList->pTail = pNode;
        *pNodeNext = NULL;
        *pNodePrev = NULL;
    }
    else // Otherwise, insert the node in the correct position
    {
        void *current = pIntoList->pHead;

        // If there's no order function, insert at the end
        if (pIntoList->orderFunction == NULL)
        {
            // Get pointers to the next and previous nodes in the list
            void **pTailNext = GetNextPointer(pIntoList->pTail, pIntoList->offset);
            void **pTailPrev = GetPrevPointer(pIntoList->pTail, pIntoList->offset);

            // set the next pointer of the current tail to the new node
            *pTailNext = pNode;
            // set the previous pointer of the new node to the current tail
            *pNodePrev = pIntoList->pTail;
            // set the tail of the list to the new node
            pIntoList->pTail = pNode;
            // set the next pointer of the new node to NULL
            *pNodeNext = NULL;
        }
        // If the order function is provided and the new node should be inserted at the head
        else if (pIntoList->orderFunction(pNode, pIntoList->pHead) < 0)
        {
            // Get the previous pointer of the current head
            void **pHeadPrev = GetPrevPointer(pIntoList->pHead, pIntoList->offset);

            // set the next pointer of the new node to the current head
            *pNodeNext = pIntoList->pHead;
            // set the previous pointer of the current head to the new node
            *pHeadPrev = pNode;
            // set the head of the list to the new node
            pIntoList->pHead = pNode;
            // set the previous pointer of the new node to NULL
            *pNodePrev = NULL;
        }
        // If the order function is provided and the new node should be inserted at the tail
        else if (pIntoList->orderFunction(pNode, pIntoList->pTail) > 0)
        {
            // Get the next pointer of the current tail
            void **pTailNext = GetNextPointer(pIntoList->pTail, pIntoList->offset);

            // set the next pointer of the current tail to the new node
            *pTailNext = pNode;
            // set the previous pointer of the new node to the current tail
            *pNodePrev = pIntoList->pTail;
            // set the tail of the list to the new node
            pIntoList->pTail = pNode;
            // set the next pointer of the new node to NULL
            *pNodeNext = NULL;
        }
        else
        {
            // Insert in the middle
            // look for a spot to insert the new node
            while (current != NULL)
            {
                if (pIntoList->orderFunction(pNode, current) < 0)
                    break;

                current = *GetNextPointer(current, pIntoList->offset);
            }

            // Should not happen, we insert at the tail above, but a safety check
            if (current == NULL)
            {
                // Get pointers to the next and previous nodes in the list
                void **pPrevNext = GetNextPointer(pIntoList->pTail, pIntoList->offset);
                void **pTailPrev = GetPrevPointer(pIntoList->pTail, pIntoList->offset);

                // set the previous pointer of the new node to the current tail
                *pNodePrev = pIntoList->pTail;
                // set the next pointer of the current tail to the new node
                *pPrevNext = pNode;
                // set the tail of the list to the new node
                pIntoList->pTail = pNode;
                // set the next pointer of the new node to NULL
                *pNodeNext = NULL;
            }
            else
            {
                // Get the previous node
                void *prev = *GetPrevPointer(current, pIntoList->offset);
                // Get the next pointer of the previous node
                void **pPrevNext = GetNextPointer(prev, pIntoList->offset);
                // Get the previous pointer of the current node
                void **pCurrentPrev = GetPrevPointer(current, pIntoList->offset);

                // set the next pointer of the new node to the current node
                *pNodeNext = current;
                // set the previous pointer of the new node to the previous node
                *pNodePrev = prev;
                // set the next pointer of the previous node to the new node
                *pPrevNext = pNode;
                // set the previous pointer of the current node to the new node
                *pCurrentPrev = pNode;
            }
        }
    }

    pIntoList->count++;
}

/**
 * @brief Creates a dynamic doubly linked node.
 *
 * This function creates a dynamically allocated doubly linked node.
 *
 * @param pData Pointer to the data the node holds.
 * @return Pointer to the created DoublyLinkedNode structure.
 */
DynamicDoublyLinkedNode *CreateDoublyLinkedNode(void *pData)
{
    DynamicDoublyLinkedNode *pNode =
        (DynamicDoublyLinkedNode *)malloc(sizeof(DynamicDoublyLinkedNode));

    if (pNode == NULL)
    {
        return NULL;
    }

    InitializeDoublyLinkedNode(1, pNode, pData);
    return pNode;
}

/**
 * @brief Destroys a doubly linked list.
 *
 * This function destroys a doubly linked list.
 * If the cleanNodes flag is set to 1, it will also
 * destroy the nodes in the list.
 *
 * @param pList Pointer to the DoublyLinkedList structure.
 * @param cleanNodes Flag to indicate if the nodes should be destroyed,
 *                   TRUE if they should, FALSE otherwise.
 * @return Nothing
 * @note If the list or nodes are dynamically allocated, they will be freed
 *       otherwise they will be reset.
 */
void DestroyDoublyLinkedList(DoublyLinkedList *pList, int cleanNodes)
{
    if (pList == NULL)
    {
        return;
    }

    DoublyLinkedNode *pNode = pList->pHead;

    if (cleanNodes == 1)
    {
        while (pNode != NULL)
        {
            // Save the next node
            DoublyLinkedNode *pNext = pNode->pNext;
            // Destroy the current node
            DestroyDoublyLinkedNode(pNode);
            // Move to the next node
            pNode = pNext;
        }
    }

    if (pList->dynamic == 1)
    {
        free(pList);
    }
    else
    {
        InitializeDoublyLinkedList(0, pList->offset, pList, NULL);
    }
}

/**
 * @brief Initializes a doubly linked node.
 *
 * This function initializes a doubly linked node.
 *
 * @param pNode Pointer to the DoublyLinkedNode structure.
 * @param pWithData Pointer to the data the node holds.
 * @param isDynamic Flag to indicate if the node is dynamically allocated,
 *                  TRUE if it is, FALSE otherwise.
 * @return Nothing
 */
void InitializeDoublyLinkedNode(int isDynamic, DoublyLinkedNode *pNode, void *pWithData)
{
    if (pNode == NULL)
    {
        return;
    }

    pNode->pNext = NULL;
    pNode->pPrev = NULL;
    pNode->pData = pWithData;
    pNode->dynamic = isDynamic;
}

/**
 * @brief Creates a dynamic doubly linked list.
 *
 * This function creates a dynamically allocated doubly linked list.
 *
 * @param offset Offset to the pNext field in the data structure.
 * @param pOrderFunction Pointer to the function used to order the list,
 *                       can be NULL.
 * @return Pointer to the created DoublyLinkedList structure.
 */
DynamicDoublyLinkedList *CreateDoublyLinkedList(size_t offset, OrderFunction pOrderFunction)
{
    DynamicDoublyLinkedList *pList =
        (DynamicDoublyLinkedList *)malloc(sizeof(DynamicDoublyLinkedList));

    if (pList == NULL)
    {
        return NULL;
    }

    InitializeDoublyLinkedList(1, offset, pList, pOrderFunction);
    return pList;
}

/**
 * @brief Initializes a doubly linked list.
 *
 * This function initializes a doubly linked list.
 *
 * @param isDynamic Flag to indicate if the list is dynamically allocated,
 *                 TRUE if it is, FALSE otherwise.
 * @param offset Offset to the pNext field in the data structure.
 * @param pList Pointer to the DoublyLinkedList structure.
 * @param pOrderFunction Pointer to the function used to order the list,
 *                      can be NULL.
 * @return Nothing
 */
void InitializeDoublyLinkedList(int isDynamic, size_t offset, DoublyLinkedList *pList, OrderFunction pOrderFunction)
{
    if (pList == NULL)
    {
        return;
    }

    pList->count = 0;
    pList->pHead = NULL;
    pList->pTail = NULL;
    pList->dynamic = isDynamic;
    pList->orderFunction = pOrderFunction;
    pList->offset = offset == -1 ? DOUBLY_LINKED_NODE_OFFSET : offset;
}
