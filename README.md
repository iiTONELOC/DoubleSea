# DoubleSeaLib

    +-------------------------+
    |         DSL_Node        |
    +-------------------------+
    | pData   |    (void *)   |
    | pNext   |    (void *)   |
    | pPrev   |    (void *)   |
    | dynamic |      int      |
    +-------------------------+

    +-----------------------------------+
    |              DSL_List             |
    +-----------------------------------+
    | pHead         |      (void *)     |
    | pTail         |      (void *)     |
    | dynamic       |        int        |
    | count         |       size_t      |
    | offset        |       size_t      |
    | orderFunction |    Function Ptr   |
    +-----------------------------------+
---

## Overview

This file contains the implementation for a generic doubly linked list in C.  

The implementation supports basic linked list operations such as initialization, insertion, and removal.

The linked list is designed to store data in nodes, with the ability to insert and remove nodes in a sorted order based on a user-defined comparison function. The List supports the `DSL_Node` struct, but also will accept any structure provided the pPrev pointer comes after the pNext pointer.

Support is also provided for dynamic lists and nodes via the dynamic flag. When this is set and destroy operations are called, only nodes that have been marked as dynamic will be freed. All other nodes will be reset to default values.
