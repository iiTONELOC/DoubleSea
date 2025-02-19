#include "ListUtils.h"

/**
 * @brief Gets the pointer to the next node in a doubly linked list.
 *
 * This function gets the pointer to the next node in a doubly linked list.
 *
 * @param pNode Pointer to the current node.
 * @param offset Offset to the pNext field in the data structure.
 * @return Pointer to the next node.
 */
void **GetNextPointer(void *pNode, size_t offset)
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
void **GetPrevPointer(void *pNode, size_t offset)
{
    return (void **)((char *)pNode + offset + sizeof(void *));
}

/**
 * @brief Calculates the offset to the next pointer in a doubly linked list.
 *
 * This function calculates the offset to the next pointer in a doubly linked list.
 *
 * @param pNode Pointer to the current node.
 * @param offset Offset to the pNext field in the data structure.
 * @return Offset to the next pointer.
 */
size_t CalculateNextPointerOffset(void *pNode, size_t offset)
{
    return (size_t)((char *)pNode + offset);
}

/**
 * @brief Calculates the offset to the previous pointer in a doubly linked list.
 *
 * This function calculates the offset to the previous pointer in a doubly linked list.
 *
 * @param pNode Pointer to the current node.
 * @param offset Offset to the pPrev field in the data structure.
 * @return Offset to the previous pointer.
 */
size_t CalculatePrevPointerOffset(void *pNode, size_t offset)
{
    return (size_t)((char *)pNode + offset + sizeof(void *));
}
