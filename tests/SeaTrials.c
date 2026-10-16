#include <stdio.h>
#undef NDEBUG
#include <assert.h>
#include "DoubleSeaLib.h"

typedef struct testData
{
	int number;
} TestData;

int orderFunction(void* pNode1, void* pNode2);
int compareFunction(void* pNode1, void* pNode2, size_t offset);
void testInitDoublyLinkedList();
void testInitDoublyLinkedNode();
void testInitDynamicDoublyLinkedList();
void testInitDynamicDoublyLinkedNode();
void testInsertNode();
void testRemoveNode();
void testPushNode();
void testPopNode();
void testFindNode();
void testFindNodeMiss();
void testRemoveNonMember();
void testInsertTwice();
void testDestroyCustomStruct();

void (*testFunctions[])() = {
	testInitDoublyLinkedList,
	testInitDoublyLinkedNode,
	testInitDynamicDoublyLinkedList,
	testInitDynamicDoublyLinkedNode,
	testInsertNode,
	testRemoveNode,
	testPushNode,
	testPopNode,
	testFindNode,
	testFindNodeMiss,
	testRemoveNonMember,
	testInsertTwice,
	testDestroyCustomStruct };

TestData testNumbers[5] = { {1}, {2}, {3}, {4}, {5} };
DSL_List testList = { 0, 0, 0, 0, OFFSETOF_DSL_NODE, orderFunction };
DSL_Node nodeBucket[5] = { {&testNumbers[0], NULL, NULL, 0},
						  {&testNumbers[1], NULL, NULL, 0},
						  {&testNumbers[2], NULL, NULL, 0},
						  {&testNumbers[3], NULL, NULL, 0},
						  {&testNumbers[4], NULL, NULL, 0} };

int main()
{
	printf("Running tests for DoublyLinkedList.c\n");
	for (size_t i = 0; i < sizeof(testFunctions) / sizeof(testFunctions[0]); i++)
	{
		testFunctions[i]();
	}
	printf("All tests passed\n");
	return 0;
}

/**
 * @brief Compare function for the test data.
 *
 * @param pNode1 The first node to compare.
 * @param pNode2 The second node to compare.
 * @param offset The offset to the data in the node.
 *
 * @return The difference between the two numbers.
 */
int compareFunction(void* pNode1, void* pNode2, size_t offset)
{
	if (pNode1 == NULL || pNode2 == NULL)
	{
		return 0;
	}

	TestData* data1 = (TestData*)DSL_GetDataPointer(pNode1, offset);
	TestData* data2 = (TestData*)DSL_GetDataPointer(pNode2, offset);

	if (data1 == NULL || data2 == NULL)
	{
		return 0;
	}

	return data1->number - data2->number;
}

/**
 * @brief Order function for the test data.
 *
 * @param pNode1 The first node to compare.
 * @param pNode2 The second node to compare.
 *
 * @return The difference between the two numbers.
 */
int orderFunction(void* pNode1, void* pNode2)
{
	if (pNode1 == NULL || pNode2 == NULL)
	{
		return 0;
	}

	TestData* data1 = (TestData*)((DSL_Node*)pNode1)->pData;
	TestData* data2 = (TestData*)((DSL_Node*)pNode2)->pData;

	if (data1 == NULL || data2 == NULL)
	{
		return 0;
	}

	return data1->number - data2->number;
}

void testInitDoublyLinkedList()
{
	DSL_List list;
	DSL_InitList(0, OFFSETOF_DSL_NODE, &list, NULL);
	assert(list.length == 0);
	assert(list.dynamic == 0);
	assert(list.pHead == NULL);
	assert(list.pTail == NULL);
	assert(list.orderFunction == NULL);
	printf("  Test 1 - Init Doubly Linked List - passed\n");
}

void testInitDoublyLinkedNode()
{
	DSL_Node node;
	DSL_InitNode(0, &node, NULL);
	assert(node.pData == NULL);
	assert(node.pNext == NULL);
	assert(node.pPrev == NULL);
	assert(node.dynamic == 0);
	printf("  Test 2 - Init Doubly Linked Node - passed\n");
}

void testInitDynamicDoublyLinkedList()
{
	DSL_List list;
	DSL_InitList(1, OFFSETOF_DSL_NODE, &list, NULL);
	assert(list.length == 0);
	assert(list.dynamic == 1);
	assert(list.pHead == NULL);
	assert(list.pTail == NULL);
	assert(list.orderFunction == NULL);
	printf("  Test 3 - Init Dynamic Doubly Linked List - passed\n");
}

void testInitDynamicDoublyLinkedNode()
{
	DSL_Node node;
	DSL_InitNode(1, &node, NULL);
	assert(node.pData == NULL);
	assert(node.pNext == NULL);
	assert(node.pPrev == NULL);
	assert(node.dynamic == 1);
	printf("  Test 4 - Init Dynamic Doubly Linked Node - passed\n");
}


void testInsertNode()
{
	// insert the nodes
	for (size_t i = 0; i < 5; i++)
	{
		DSL_InsertNode(&nodeBucket[i], &testList);

		assert(testList.length == i + 1);
		assert(testList.pHead == &nodeBucket[0]);
		assert(testList.pTail == &nodeBucket[i]);
		assert(testList.orderFunction == orderFunction);
	}
	printf("  Test 5 - Insert Node - passed\n");
}

void testRemoveNode()
{
	// remove the nodes
	for (int i = 4; i >= 0; i--)
	{
		DSL_RemoveNode(&nodeBucket[i], &testList);

		assert(testList.length == (size_t)i);
		if (i == 0)
		{
			assert(testList.pHead == NULL);
			assert(testList.pTail == NULL);
		}
		else
		{
			assert(testList.pHead == &nodeBucket[0]);
			assert(testList.pTail == &nodeBucket[i - 1]);
		}
		assert(testList.orderFunction == orderFunction);
	}
	printf("  Test 6 - Remove Node - passed\n");
}

void testPushNode()
{
	// insert the nodes
	for (size_t i = 0; i < 5; i++)
	{
		DSL_Push(&nodeBucket[i], &testList);

		assert(testList.length == i + 1);
		assert(testList.pHead == &nodeBucket[0]);
		assert(testList.pTail == &nodeBucket[i]);
		assert(testList.orderFunction == orderFunction);
	}
	printf("  Test 7 - Push Node - passed\n");
}

void testPopNode()
{
	// remove the nodes
	for (int i = 4; i >= 0; i--)
	{
		DSL_Node* node = DSL_Pop(&testList);

		assert(node == &nodeBucket[4 - i]);
		assert(testList.length == (size_t)i);
		if (i == 0)
		{
			assert(testList.pHead == NULL);
			assert(testList.pTail == NULL);
		}
		else
		{
			assert(testList.pHead == &nodeBucket[5 - i]);
			assert(testList.pTail == &nodeBucket[4]);
		}
		assert(testList.orderFunction == orderFunction);
	}
	printf("  Test 8 - Pop Node - passed\n");
}

void testFindNode()
{
	// insert the nodes
	for (size_t i = 0; i < 5; i++)
	{
		DSL_InsertNode(&nodeBucket[i], &testList);
	}

	// find the nodes
	for (size_t i = 0; i < 5; i++)
	{
		DSL_Node* node = (DSL_Node*)*DSL_FindNode(&testList, &testNumbers[i]);
		assert(node == &nodeBucket[i]);
	}
	printf("  Test 9 - Find Node - passed\n");
}

void testFindNodeMiss()
{
	TestData outside = { 99 };
	assert(DSL_FindNode(&testList, &outside) == NULL);

	DSL_List empty;
	DSL_InitList(0, OFFSETOF_DSL_NODE, &empty, NULL);
	assert(DSL_FindNode(&empty, &testNumbers[0]) == NULL);
	printf("  Test 10 - Find Node Miss - passed\n");
}

void testRemoveNonMember()
{
	DSL_Node outside = { &testNumbers[0], NULL, NULL, 0 };
	DSL_RemoveNode(&outside, &testList);

	assert(testList.length == 5);
	assert(testList.pHead == &nodeBucket[0]);
	assert(testList.pTail == &nodeBucket[4]);
	printf("  Test 11 - Remove Non-Member - passed\n");
}

void testInsertTwice()
{
	DSL_InsertNode(&nodeBucket[0], &testList);
	DSL_Push(&nodeBucket[4], &testList);

	assert(testList.length == 5);
	assert(testList.pHead == &nodeBucket[0]);
	assert(testList.pTail == &nodeBucket[4]);
	assert(nodeBucket[0].pPrev == NULL);
	assert(nodeBucket[4].pNext == NULL);
	printf("  Test 12 - Insert Twice - passed\n");
}

typedef struct customNode
{
	int tag;
	void* pData;
	void* pNext;
	void* pPrev;
} CustomNode;

void testDestroyCustomStruct()
{
	CustomNode nodes[3] = { {1, &testNumbers[0], NULL, NULL},
						   {2, &testNumbers[1], NULL, NULL},
						   {3, &testNumbers[2], NULL, NULL} };
	DSL_List list;
	DSL_InitList(0, offsetof(CustomNode, pNext), &list, NULL);
	for (int i = 0; i < 3; i++)
	{
		DSL_InsertNode(&nodes[i], &list);
	}
	assert(list.length == 3);

	DSL_DestroyList(&list, 1);

	assert(list.length == 0);
	assert(list.pHead == NULL);
	assert(list.pTail == NULL);
	for (int i = 0; i < 3; i++)
	{
		assert(nodes[i].pNext == NULL);
		assert(nodes[i].pPrev == NULL);
		assert(nodes[i].pData == NULL);
		assert(nodes[i].tag == i + 1);
	}
	printf("  Test 13 - Destroy Custom Struct - passed\n");
}
