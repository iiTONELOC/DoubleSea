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
    | length        |       size_t      |
    | offset        |       size_t      |
    | orderFunction |    Function Ptr   |
    +-----------------------------------+
---

## Overview

A generic intrusive doubly linked list in C. It depends on `stddef.h` only, so it builds freestanding for kernel and embedded targets.

The list supports insert, remove, push, pop, find, and destroy. When an order function is set, insert keeps the list sorted. Push appends at the tail and pop removes the head.

The list accepts `DSL_Node` or any structure that lays out `pData`, `pNext`, and `pPrev` as three consecutive pointers in that order. Pass the byte offset of `pNext` to `DSL_InitList`, or `DSL_DEFAULT_OFFSET` for `DSL_Node`. `DSL_GetNextPointer`, `DSL_GetPrevPointer`, and `DSL_GetDataPointer` read those fields through the offset.

The library never allocates or frees. Destroy unlinks nodes and resets them to default values. The `dynamic` flag is a marker the caller can read to decide what to free.

## Layout

    include/   DoubleSeaLib.h, the public header
    src/       DoubleSeaLib.c
    src/win/   dllmain.c and the precompiled header, Windows only
    tests/     SeaTrials.c and its Visual Studio project
    msvc/      Visual Studio solution and library project
    Makefile   Linux and macOS build

## Build

Linux and macOS:

    make         builds build/libDoubleSeaLib.a and build/libDoubleSeaLib.so
    make test    builds and runs SeaTrials
    make install PREFIX=/usr/local
    make clean

Windows: open `msvc/DoubleSeaLib.sln` and build. The solution contains the library and the SeaTrials test.
