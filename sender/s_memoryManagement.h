//
// Created by michi on 5/28/23.
//

#ifndef SHAREDMEMORY_S_MEMORYMANAGEMENT_H
#define SHAREDMEMORY_S_MEMORYMANAGEMENT_H
#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"

ring_buffer * createRingBuffer(size_t bufferSize);

void cleanup(int shmId, void *shmAddr);

#endif //SHAREDMEMORY_S_MEMORYMANAGEMENT_H
