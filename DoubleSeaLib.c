#include "pch.h"
#include <malloc.h>
#include "DoubleSeaLib.h"

// __________________________ Prototypes __________________________

static void _InsertNodeAtHead(void *pNode, DSL_List *pOfList);
static void _InsertNodeAtTail(void *pNode, DSL_List *pOfList);

// __________________________ Functions __________________________

/**
 * @brief DSL_Pop removes the first node from the list and returns it
 * @param pFromList - A pointer to the list from which the node will be removed
 * @return void* - A pointer to the removed node
 */
void *DSL_Pop(DSL_List *pFromList)
{
	if (!pFromList || pFromList->length == 0)
	{
		return NULL;
	}

	void *pNode = pFromList->pHead;
	DSL_RemoveNode(pNode, pFromList); // remove node adjusts the count
	return pNode;
}

/**
 * @brief DSL_DestroyNode frees the memory of a node
 *
 * Destroys a linked list node and frees the memory that it occupies if it is dynamic.
 * Otherwise, the node is reset to default values but it's memory is not freed.
 *
 * @param pNode - A pointer to the node that will be destroyed
 */
void DSL_DestroyNode(DSL_Node *pNode)
{
	if (!pNode)
	{
		return;
	}
	else if (pNode->dynamic == 1)
	{
		free(pNode);
	}
	else
	{
		DSL_InitNode(0, pNode, NULL);
	}
}

/**
 * @brief DSL_Push adds a node to the end of the list
 *
 * Adds a node to the end of the list.
 *
 * @param pNode - A pointer to the node that will be added
 * @param pIntoList - A pointer to the list that the node will be added to
 */
void DSL_Push(void *pNode, DSL_List *pIntoList)
{
	if (!pIntoList || !pNode)
	{
		return;
	}
	_InsertNodeAtHead(pNode, pIntoList);
	pIntoList->length++;
}

/**
 * @brief DSL_RemoveNode removes a node from a list
 *
 * Removes a node from a list.
 *
 * @param pNode - A pointer to the node that will be removed
 * @param pFromList - A pointer to the list from which the node will be removed
 */
void DSL_RemoveNode(void *pNode, DSL_List *pFromList)
{
	if (!pFromList || !pNode || pFromList->length == 0)
		return;

	// Get pointers to the next and previous nodes in the list
	void **pNodeNext = _GetNextPointer(pNode, pFromList->offset);
	void **pNodePrev = _GetPrevPointer(pNode, pFromList->offset);

	// If the node is the head of the list
	if (pFromList->pHead == pNode)
	{
		pFromList->pHead = *pNodeNext;
		if (pFromList->pHead) // If there's a new head, update its prev pointer
		{
			void **pHeadPrev = _GetPrevPointer(pFromList->pHead, pFromList->offset);
			*pHeadPrev = NULL;
		}
	}

	// If the node is the tail of the list
	if (pFromList->pTail == pNode)
	{
		pFromList->pTail = *pNodePrev;
		if (pFromList->pTail) // If there's a new tail, update its next pointer
		{
			void **pTailNext = _GetNextPointer(pFromList->pTail, pFromList->offset);
			*pTailNext = NULL;
		}
	}

	// If the node is in the middle of the list
	if (*pNodePrev != NULL)
	{
		void **pPrevNext = _GetNextPointer(*pNodePrev, pFromList->offset);
		*pPrevNext = *pNodeNext;
	}

	if (*pNodeNext != NULL)
	{
		void **pNextPrev = _GetPrevPointer(*pNodeNext, pFromList->offset);
		*pNextPrev = *pNodePrev;
	}

	// Reset the removed node's pointers to avoid dangling references
	*pNodeNext = NULL;
	*pNodePrev = NULL;

	// Decrement the length
	pFromList->length--;

	// If the list is empty now, reset head and tail
	if (pFromList->length == 0)
	{
		pFromList->pHead = NULL;
		pFromList->pTail = NULL;
	}
}

/**
 * @brief DSL_InsertNode inserts a node into a list
 *
 * Inserts a node into a list.
 *
 * @param pNode - A pointer to the node that will be inserted
 * @param pIntoList - A pointer to the list that the node will be inserted into
 */
void DSL_InsertNode(void *pNode, DSL_List *pIntoList)
{
	if (pIntoList == NULL || pNode == NULL)
		return;

	// Get pointers to the next and previous nodes in the list
	void **pNodeNext = _GetNextPointer(pNode, pIntoList->offset);
	void **pNodePrev = _GetPrevPointer(pNode, pIntoList->offset);

	// If the list is empty, set head and tail to the new node
	if (pIntoList->length == 0)
	{
		pIntoList->pHead = pNode;
		pIntoList->pTail = pNode;
		*pNodeNext = NULL;
		*pNodePrev = NULL;
	}
	else // Otherwise, insert the node in the correct position
	{
		void *current = pIntoList->pHead;

		// If there's no order function or the new node should be inserted at the tail
		if (pIntoList->orderFunction == NULL || pIntoList->orderFunction(pNode, pIntoList->pTail) > 0)
		{
			_InsertNodeAtTail(pNode, pIntoList);
		}
		// If the order function is provided and the new node should be inserted at the head
		else if (pIntoList->orderFunction(pNode, pIntoList->pHead) < 0)
		{
			_InsertNodeAtHead(pNode, pIntoList);
		}
		else
		{
			// Insert in the middle
			// look for a spot to insert the new node
			while (current != NULL)
			{
				if (pIntoList->orderFunction(pNode, current) < 0)
					break;

				current = *_GetNextPointer(current, pIntoList->offset);
			}

			// Should not happen, we insert at the tail above, but a safety check
			if (current == NULL)
			{
				_InsertNodeAtTail(pNode, pIntoList);
			}
			// Insert between two nodes
			else
			{
				// Get the previous node
				void *prev = *_GetPrevPointer(current, pIntoList->offset);
				// Get the next pointer of the previous node
				void **pPrevNext = _GetNextPointer(prev, pIntoList->offset);
				// Get the previous pointer of the current node
				void **pCurrentPrev = _GetPrevPointer(current, pIntoList->offset);

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

	pIntoList->length++;
}

/**
 * @brief DSL_DestroyList destroys a list
 *
 * Destroys a list and frees the memory the struct occupies if it is dynamic.
 * Otherwise, the list is reset to default values but it's memory is not freed.
 * Destroying a list, will also destroy all of it's nodes if the cleanNodes flag is set.
 *
 * @param pList - A pointer to the list that will be destroyed
 * @param cleanNodes - A flag that indicates if the nodes of the list will be destroyed
 */
void DSL_DestroyList(DSL_List *pList, int cleanNodes)
{
	if (pList == NULL)
	{
		return;
	}

	DSL_Node *pNode = pList->pHead;

	if (cleanNodes == 1)
	{
		while (pNode != NULL)
		{
			// Save the next node
			DSL_Node *pNext = pNode->pNext;
			// Destroy the current node
			DSL_DestroyNode(pNode);
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
		DSL_InitList(0, pList->offset, pList, NULL);
	}
}

/**
 * @brief DSL_FindNode finds a node in a list
 *
 * Finds a node in a list.
 *
 * @param pList - A pointer to the list that the node will be searched in
 * @param pWithData - A pointer to the data that the node holds
 * @return void** - A pointer to the found node
 */
void **DSL_FindNode(DSL_List *pList, void *pWithData)
{
	if (!pList || !pWithData)
	{
		return NULL;
	}

	// get the head of the list
	void **pNode = &pList->pHead;

	// get the pointer to the data in the node
	void **pNodeData = _GetDataPointer(*pNode, pList->offset);

	// check if the data in the head node is the same as the data we are looking for
	if (pNodeData != NULL && *pNodeData == pWithData)
	{
		return &pList->pHead;
	}

	// check if the data in the tail node is the same as the data we are looking for
	pNode = &pList->pTail;
	pNodeData = _GetDataPointer(*pNode, pList->offset);
	if (pNodeData != NULL && *pNodeData == pWithData)
	{
		return &pList->pTail;
	}

	// traverse the list looking for the data
	pNode = &pList->pHead;
	while (pNode != NULL)
	{
		// get the next node
		pNode = _GetNextPointer(*pNode, pList->offset);
		// get the pointer to the data in the node
		pNodeData = _GetDataPointer(*pNode, pList->offset);
		// check if the data in the node is the same as the data we are looking for
		if (pNodeData != NULL && *pNodeData == pWithData)
		{
			return pNode;
		}
	}

	return NULL;
}

/**
 * @brief DSL_InitNode initializes a dynamic node
 *
 * Initializes a dynamic node with the given data.
 *
 * @param isDynamic - A flag that indicates if the node is dynamic
 * @param pNode - A pointer to the node that will be initialized
 * @param pWithData - A pointer to the data that the node will hold
 */
void DSL_InitNode(int isDynamic, DSL_Node *pNode, void *pWithData)
{
	if (!pNode)
	{
		return;
	}

	pNode->pNext = NULL;
	pNode->pPrev = NULL;
	pNode->pData = pWithData;
	pNode->dynamic = isDynamic;
}

/**
 * @brief DSL_InitStaticStorageListWData initializes a list with data
 *
 * Initializes a list using a static array of nodes. The function initializes the
 * list, adds the nodes to it, and sets each node's index into the table for easy access.
 * This requires the stuct being added to have an indexOffset field, and for its offset in
 * bytes to be passed to the `pArgs->indexOffset` field.
 *
 * @param pArgs - A pointer to the arguments that will be used to initialize the list
 */
void DSL_InitStaticStorageListWData(DSL_InitStaticStorageListArgs *pArgs)
{
	if (!pArgs)
	{
		return;
	}

	/* Loop over the array */
	for (size_t i = 0; i < pArgs->maxItems; i++)
	{
		/* set the table index using its offset */
		*(size_t *)((char *)pArgs->data + (i * pArgs->structSize) + pArgs->indexOffset) = i;
		/* get the address of the data */
		void *pData = (char *)pArgs->data + (i * pArgs->structSize);
		/* Add it to the list */
		DSL_InsertNode(pData, pArgs->pList);
	}
}

/**
 * @brief DSL_InitList initializes a list
 *
 * Initializes a list with the given offset and order function.
 *
 * @param isDynamic - A flag that indicates if the list is dynamic
 * @param offset - The offset to the pNext pointers in the nodes
 * @param pList - A pointer to the list that will be initialized
 * @param pOrderFunction - A function pointer to the function that compares two nodes
 */
void DSL_InitList(int isDynamic, size_t offset, DSL_List *pList, OrderFunction pOrderFunction)
{
	if (pList == NULL)
	{
		return;
	}

	pList->length = 0;
	pList->pHead = NULL;
	pList->pTail = NULL;
	pList->dynamic = isDynamic;
	pList->orderFunction = pOrderFunction;
	pList->offset = offset == -1 ? OFFSETOF_DSL_NODE : offset;
}

/**
 * @brief Gets the pointer to the next node in a doubly linked list.
 *
 * This function gets the pointer to the next node in a doubly linked list.
 *
 * @param pNode Pointer to the current node.
 * @param offset Offset to the pNext field in the data structure.
 * @return Pointer to the next node.
 */
void **_GetNextPointer(void *pNode, size_t offset)
{
	return (void **)((char *)pNode + offset);
}

/**
 * @brief Gets the pointer to the previous node in a doubly linked list.
 *
 * This function gets the pointer to the previous node in a doubly linked list.
 *
 * @param pNode Pointer to the current node.
 * @param offset Offset to the pPrev field in the data structure.
 * @return Pointer to the previous node.
 */
void **_GetPrevPointer(void *pNode, size_t offset)
{
	return (void **)((char *)pNode + offset + sizeof(void *));
}

/**
 * @brief Gets the pointer to the data in a doubly linked list node.
 *
 * This function gets the pointer to the data in a doubly linked list node.
 *
 * @param pNode Pointer to the current node.
 * @param offset Offset to the pNext field in the data structure.
 * @return Pointer to the data.
 */
void **_GetDataPointer(void *pNode, size_t offset)
{

	return (void **)((char *)pNode + offset - sizeof(void *));
}

// __________________________ Static Functions __________________________

/**
 * @brief Inserts a node at the head of a doubly linked list.
 *
 * This function inserts a node at the head of a doubly linked list.
 *
 * @param pNode Pointer to the node to insert.
 * @param pOfList Pointer to the list to insert the node into.
 * @return Nothing
 * @note This function does not increment the count of the list, it is assumed that the
 *       caller will increment the count if necessary.
 */
static void _InsertNodeAtHead(void *pNode, DSL_List *pOfList)
{
	// validate the input
	if (!pNode || !pOfList || pOfList->pHead == pNode)
	{
		return;
	}

	// get the next and previous pointers
	void **pNext = _GetNextPointer(pNode, pOfList->offset);
	void **pPrev = _GetPrevPointer(pNode, pOfList->offset);

	// check if the list is empty
	if (pOfList->length == 0)
	{
		// set the head and tail to the new node
		pOfList->pHead = pNode;
		pOfList->pTail = pNode;
		// set the next and previous pointers to NULL
		*pNext = NULL;
		*pPrev = NULL;
	}
	else
	{
		// set the next pointer of the new node to the current head
		*pNext = pOfList->pHead;
		// set the previous pointer of the current head to the new node
		*_GetPrevPointer(pOfList->pHead, pOfList->offset) = pNode;
		// set the head of the list to the new node
		pOfList->pHead = pNode;
		// set the previous pointer of the new node to NULL
		*pPrev = NULL;
	}
}

/**
 * @brief Inserts a node at the tail of a doubly linked list.
 *
 * This function inserts a node at the tail of a doubly linked list.
 *
 * @param pNode Pointer to the node to insert.
 * @param pOfList Pointer to the list to insert the node into.
 * @return Nothing
 * @note This function does not increment the count of the list, it is assumed
 *        that the caller will increment the count if necessary.
 */
static void _InsertNodeAtTail(void *pNode, DSL_List *pOfList)
{
	// validate the input
	if (!pNode || !pOfList || pOfList->pTail == pNode)
	{
		return;
	}

	// get the next and previous pointers of the new node
	void **pNext = _GetNextPointer(pNode, pOfList->offset);
	void **pPrev = _GetPrevPointer(pNode, pOfList->offset);
	// get the next pointer of the current tail
	void **pTailNext = _GetNextPointer(pOfList->pTail, pOfList->offset);

	// set the next pointer of the current tail to the new node
	*pTailNext = pNode;
	// set the previous pointer of the new node to the current tail
	*pPrev = pOfList->pTail;
	// set the tail of the list to the new node
	pOfList->pTail = pNode;
	// set the next pointer of the new node to NULL
	*pNext = NULL;
}
