//
// Created by michi on 5/28/23.
//

#ifndef SHAREDMEMORY_R_MEMORYMANAGEMENT_H
#define SHAREDMEMORY_R_MEMORYMANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

short create_shared_memory(key_t * key,const size_t * bufferSize,int * shmid_sharedMemoryID,void ** shmaddr_sharedMemoryAddress);

short ringbuffer_init(ring_buffer * buf, size_t bufferSize);

void cleanup(int shmId, void *shmAddr);

#endif //SHAREDMEMORY_R_MEMORYMANAGEMENT_H
