#pragma once
#ifndef DOUBLE_SEA_UTILS_H
#define DOUBLE_SEA_UTILS_H

#include <stddef.h> // For offsetof

void **GetNextPointer(void *pNode, size_t offset);
void **GetPrevPointer(void *pNode, size_t offset);
size_t CalculateNextPointerOffset(void *pNode, size_t offset);
size_t CalculatePrevPointerOffset(void *pNode, size_t offset);

#endif