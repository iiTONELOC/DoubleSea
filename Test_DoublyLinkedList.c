#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "DoublyLinkedList.h"

typedef struct testData
{
    int number;
} TestData;

int orderFunction(void *pNode1, void *pNode2);
int compareFunction(void *pNode1, void *pNode2, size_t offset);
void testInitDoublyLinkedList();
void testInitDoublyLinkedNode();
void testInitDynamicDoublyLinkedList();
void testInitDynamicDoublyLinkedNode();
void testCreateDoublyLinkedNode();
void testCreateDoublyLinkedList();
void testInsertNode();
void testRemoveNode();
void testPushNode();
void testPopNode();
void testFindNode();

void (*testFunctions[])() = {
    testInitDoublyLinkedList,
    testInitDoublyLinkedNode,
    testInitDynamicDoublyLinkedList,
    testInitDynamicDoublyLinkedNode,
    testCreateDoublyLinkedList,
    testCreateDoublyLinkedNode,
    testInsertNode,
    testRemoveNode,
    testPushNode,
    testPopNode,
    testFindNode};

TestData testNumbers[5] = {{1}, {2}, {3}, {4}, {5}};
DoublyLinkedList testList = {0, 0, 0, 0, DOUBLY_LINKED_NODE_OFFSET, orderFunction};
DoublyLinkedNode nodeBucket[5] = {{&testNumbers[0], NULL, NULL, 0},
                                  {&testNumbers[1], NULL, NULL, 0},
                                  {&testNumbers[2], NULL, NULL, 0},
                                  {&testNumbers[3], NULL, NULL, 0},
                                  {&testNumbers[4], NULL, NULL, 0}};

int main()
{
    printf("Running tests for DoublyLinkedList.c\n");
    for (int i = 0; i < sizeof(testFunctions) / sizeof(testFunctions[0]); i++)
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
int compareFunction(void *pNode1, void *pNode2, size_t offset)
{
    if (pNode1 == NULL || pNode2 == NULL)
    {
        return 0;
    }

    TestData *data1 = (TestData *)GetDataPointer(pNode1, offset);
    TestData *data2 = (TestData *)GetDataPointer(pNode2, offset);

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
int orderFunction(void *pNode1, void *pNode2)
{
    if (pNode1 == NULL || pNode2 == NULL)
    {
        return 0;
    }

    TestData *data1 = (TestData *)((DoublyLinkedNode *)pNode1)->pData;
    TestData *data2 = (TestData *)((DoublyLinkedNode *)pNode2)->pData;

    if (data1 == NULL || data2 == NULL)
    {
        return 0;
    }

    return data1->number - data2->number;
}

void testInitDoublyLinkedList()
{
    DoublyLinkedList list;
    InitializeDoublyLinkedList(0, DOUBLY_LINKED_NODE_OFFSET, &list, NULL);
    assert(list.count == 0);
    assert(list.dynamic == 0);
    assert(list.pHead == NULL);
    assert(list.pTail == NULL);
    assert(list.orderFunction == NULL);
    printf("  Test 1 - Init Doubly Linked List - passed\n");
}

void testInitDoublyLinkedNode()
{
    DoublyLinkedNode node;
    InitializeDoublyLinkedNode(0, &node, NULL);
    assert(node.pData == NULL);
    assert(node.pNext == NULL);
    assert(node.pPrev == NULL);
    assert(node.dynamic == 0);
    printf("  Test 2 - Init Doubly Linked Node - passed\n");
}

void testInitDynamicDoublyLinkedList()
{
    DoublyLinkedList list;
    InitializeDoublyLinkedList(1, DOUBLY_LINKED_NODE_OFFSET, &list, NULL);
    assert(list.count == 0);
    assert(list.dynamic == 1);
    assert(list.pHead == NULL);
    assert(list.pTail == NULL);
    assert(list.orderFunction == NULL);
    printf("  Test 3 - Init Dynamic Doubly Linked List - passed\n");
}

void testInitDynamicDoublyLinkedNode()
{
    DoublyLinkedNode node;
    InitializeDoublyLinkedNode(1, &node, NULL);
    assert(node.pData == NULL);
    assert(node.pNext == NULL);
    assert(node.pPrev == NULL);
    assert(node.dynamic == 1);
    printf("  Test 4 - Init Dynamic Doubly Linked Node - passed\n");
}

void testCreateDoublyLinkedNode()
{
    DoublyLinkedNode *node = CreateDoublyLinkedNode(NULL);
    assert(node->pData == NULL);
    assert(node->pNext == NULL);
    assert(node->pPrev == NULL);
    assert(node->dynamic == 1);
    printf("  Test 5 - Create Dynamically Allocated Doubly Linked Node - passed\n");
    DestroyDoublyLinkedNode(node);
    printf("  Test 6 - Destroy Dynamically Allocated Doubly Linked Node - passed\n");
}

void testCreateDoublyLinkedList()
{
    DoublyLinkedList *list = CreateDoublyLinkedList(-1, NULL);
    assert(list->count == 0);
    assert(list->dynamic == 1);
    assert(list->pHead == NULL);
    assert(list->pTail == NULL);
    assert(list->orderFunction == NULL);
    printf("  Test 7 - Create Dynamically Allocated Doubly Linked List - passed\n");
    DestroyDoublyLinkedList(list, 1);
    printf("  Test 8 - Destroy Dynamically Allocated Doubly Linked List - passed\n");
}

void testInsertNode()
{
    // insert the nodes
    for (int i = 0; i < 5; i++)
    {
        InsertNode(&nodeBucket[i], &testList);

        assert(testList.count == i + 1);
        assert(testList.pHead == &nodeBucket[0]);
        assert(testList.pTail == &nodeBucket[i]);
        assert(testList.orderFunction == orderFunction);
    }
    printf("  Test 9 - Insert Node - passed\n");
}

void testRemoveNode()
{
    // remove the nodes
    for (int i = 4; i >= 0; i--)
    {
        RemoveNode(&nodeBucket[i], &testList);

        assert(testList.count == i);
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
    printf("  Test 10 - Remove Node - passed\n");
}

void testPushNode()
{
    // insert the nodes
    for (int i = 0; i < 5; i++)
    {
        Push(&nodeBucket[i], &testList);

        assert(testList.count == i + 1);
        assert(testList.pHead == &nodeBucket[i]);
        assert(testList.pTail == &nodeBucket[0]);
        assert(testList.orderFunction == orderFunction);
    }
    printf("  Test 11 - Push Node - passed\n");
}

void testPopNode()
{
    // remove the nodes
    for (int i = 4; i >= 0; i--)
    {
        DoublyLinkedNode *node = Pop(&testList);

        assert(testList.count == i);
        if (i == 0)
        {
            assert(testList.pHead == NULL);
            assert(testList.pTail == NULL);
        }
        else
        {
            assert(testList.pHead == &nodeBucket[i - 1]);
            assert(testList.pTail == &nodeBucket[0]);
        }
        assert(testList.orderFunction == orderFunction);
    }
    printf("  Test 12 - Pop Node - passed\n");
}

void testFindNode()
{
    // insert the nodes
    for (int i = 0; i < 5; i++)
    {
        InsertNode(&nodeBucket[i], &testList);
    }

    // find the nodes
    for (int i = 0; i < 5; i++)
    {
        DoublyLinkedNode *node = *FindDoublyLinkedNode(&testList, &nodeBucket[i]);
        assert(node == &nodeBucket[i]);
    }
    printf("  Test 13 - Find Node - passed\n");
}
