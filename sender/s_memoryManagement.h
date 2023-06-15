//
// Created by michi on 5/28/23.
//

#ifndef SHAREDMEMORY_S_MEMORYMANAGEMENT_H
#define SHAREDMEMORY_S_MEMORYMANAGEMENT_H
#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

ring_buffer * createRingBuffer(size_t bufferSize);

short create_shared_memory(key_t * key,const size_t * bufferSize,int * shmid_sharedMemoryID,void ** shmaddr_sharedMemoryAddress);

void cleanup(int shmId, void *shmAddr);

#endif //SHAREDMEMORY_S_MEMORYMANAGEMENT_H
