#include "DoubleSeaLib.h"

static int IsLinked(void *pNode, DSL_List *pList);
static void *FindInsertPosition(void *pNode, DSL_List *pList);
static void InsertNodeAtHead(void *pNode, DSL_List *pOfList);
static void InsertNodeAtTail(void *pNode, DSL_List *pOfList);
static void InsertNodeBefore(void *pNode, void *pCurrent, DSL_List *pOfList);

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
	DSL_RemoveNode(pNode, pFromList);
	return pNode;
}

/**
 * @brief DSL_DestroyNode resets a node
 *
 * Resets a node to default values. The caller owns the node's memory.
 *
 * @param pNode - A pointer to the node that will be destroyed
 */
void DSL_DestroyNode(DSL_Node *pNode)
{
	if (!pNode)
	{
		return;
	}
	else
	{
		DSL_InitNode(pNode->dynamic ? 1 : 0, pNode, NULL);
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
	if (!pIntoList || !pNode || IsLinked(pNode, pIntoList))
	{
		return;
	}
	InsertNodeAtTail(pNode, pIntoList);
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
	if (!pFromList || !pNode || !IsLinked(pNode, pFromList))
		return;

	void **pNodeNext = DSL_GetNextPointer(pNode, pFromList->offset);
	void **pNodePrev = DSL_GetPrevPointer(pNode, pFromList->offset);

	if (pFromList->pHead == pNode)
		pFromList->pHead = *pNodeNext;
	if (pFromList->pTail == pNode)
		pFromList->pTail = *pNodePrev;
	if (*pNodePrev != NULL)
		*DSL_GetNextPointer(*pNodePrev, pFromList->offset) = *pNodeNext;
	if (*pNodeNext != NULL)
		*DSL_GetPrevPointer(*pNodeNext, pFromList->offset) = *pNodePrev;

	*pNodeNext = NULL;
	*pNodePrev = NULL;
	pFromList->length--;
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
	if (pIntoList == NULL || pNode == NULL || IsLinked(pNode, pIntoList))
		return;

	void *current = FindInsertPosition(pNode, pIntoList);
	if (current == NULL)
		InsertNodeAtTail(pNode, pIntoList);
	else if (current == pIntoList->pHead)
		InsertNodeAtHead(pNode, pIntoList);
	else
		InsertNodeBefore(pNode, current, pIntoList);

	pIntoList->length++;
}

/**
 * @brief DSL_DestroyList destroys a list
 *
 * Resets a list to default values. The caller owns the list's memory.
 * Destroying a list will also destroy all of its nodes if the cleanNodes flag is set.
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

	if (cleanNodes == 1)
	{
		void *pNode = pList->pHead;
		while (pNode != NULL)
		{
			void *pNext = *DSL_GetNextPointer(pNode, pList->offset);
			*DSL_GetNextPointer(pNode, pList->offset) = NULL;
			*DSL_GetPrevPointer(pNode, pList->offset) = NULL;
			*DSL_GetDataPointer(pNode, pList->offset) = NULL;
			pNode = pNext;
		}
	}

	DSL_InitList(pList->dynamic ? 1 : 0, pList->offset, pList, NULL);
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

	void **pSlot = &pList->pHead;
	while (*pSlot != NULL)
	{
		if (*DSL_GetDataPointer(*pSlot, pList->offset) == pWithData)
		{
			return pSlot;
		}
		pSlot = DSL_GetNextPointer(*pSlot, pList->offset);
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

	for (size_t i = 0; i < pArgs->maxItems; i++)
	{
		*(size_t *)((char *)pArgs->data + (i * pArgs->structSize) + pArgs->indexOffset) = i;
		void *pData = (char *)pArgs->data + (i * pArgs->structSize);
		DSL_InsertNode(pData, pArgs->pList);
	}
}

/**
 * @brief DSL_InitList initializes a list
 *
 * Initializes a list with the given offset and order function.
 *
 * @param isDynamic - A flag that indicates if the list is dynamic
 * @param offset - The offset to the pNext pointers in the nodes, or DSL_DEFAULT_OFFSET for DSL_Node
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
	pList->offset = offset == DSL_DEFAULT_OFFSET ? OFFSETOF_DSL_NODE : offset;
}

/**
 * @brief Gets the pointer to the next node in a doubly linked list.
 *
 * @param pNode Pointer to the current node.
 * @param offset Offset to the pNext field in the data structure.
 * @return Pointer to the next node.
 */
void **DSL_GetNextPointer(void *pNode, size_t offset)
{
	return (void **)((char *)pNode + offset);
}

/**
 * @brief Gets the pointer to the previous node in a doubly linked list.
 *
 * @param pNode Pointer to the current node.
 * @param offset Offset to the pPrev field in the data structure.
 * @return Pointer to the previous node.
 */
void **DSL_GetPrevPointer(void *pNode, size_t offset)
{
	return (void **)((char *)pNode + offset + sizeof(void *));
}

/**
 * @brief Gets the pointer to the data in a doubly linked list node.
 *
 * @param pNode Pointer to the current node.
 * @param offset Offset to the pNext field in the data structure.
 * @return Pointer to the data.
 */
void **DSL_GetDataPointer(void *pNode, size_t offset)
{
	return (void **)((char *)pNode + offset - sizeof(void *));
}

static int IsLinked(void *pNode, DSL_List *pList)
{
	return pList->pHead == pNode
		|| *DSL_GetNextPointer(pNode, pList->offset) != NULL
		|| *DSL_GetPrevPointer(pNode, pList->offset) != NULL;
}

static void *FindInsertPosition(void *pNode, DSL_List *pList)
{
	if (pList->orderFunction == NULL)
	{
		return NULL;
	}
	for (void *current = pList->pHead; current != NULL; current = *DSL_GetNextPointer(current, pList->offset))
	{
		if (pList->orderFunction(pNode, current) < 0)
		{
			return current;
		}
	}
	return NULL;
}

static void InsertNodeAtHead(void *pNode, DSL_List *pOfList)
{
	*DSL_GetNextPointer(pNode, pOfList->offset) = pOfList->pHead;
	*DSL_GetPrevPointer(pNode, pOfList->offset) = NULL;
	if (pOfList->pHead != NULL)
	{
		*DSL_GetPrevPointer(pOfList->pHead, pOfList->offset) = pNode;
	}
	else
	{
		pOfList->pTail = pNode;
	}
	pOfList->pHead = pNode;
}

static void InsertNodeAtTail(void *pNode, DSL_List *pOfList)
{
	*DSL_GetPrevPointer(pNode, pOfList->offset) = pOfList->pTail;
	*DSL_GetNextPointer(pNode, pOfList->offset) = NULL;
	if (pOfList->pTail != NULL)
	{
		*DSL_GetNextPointer(pOfList->pTail, pOfList->offset) = pNode;
	}
	else
	{
		pOfList->pHead = pNode;
	}
	pOfList->pTail = pNode;
}

static void InsertNodeBefore(void *pNode, void *pCurrent, DSL_List *pOfList)
{
	void *pPrev = *DSL_GetPrevPointer(pCurrent, pOfList->offset);
	*DSL_GetNextPointer(pNode, pOfList->offset) = pCurrent;
	*DSL_GetPrevPointer(pNode, pOfList->offset) = pPrev;
	*DSL_GetNextPointer(pPrev, pOfList->offset) = pNode;
	*DSL_GetPrevPointer(pCurrent, pOfList->offset) = pNode;
}
