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
    // Casting to char allows us to add the offset in bytes
    // rather than in multiples of the size of the data structure
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
    // Casting to char allows us to add the offset in bytes
    // rather than in multiples of the size of the data structure
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
void **GetDataPointer(void *pNode, size_t offset)
{
    // the data pointer comes before the pNext pointer
    return (void **)((char *)pNode + offset - sizeof(void *));
}
