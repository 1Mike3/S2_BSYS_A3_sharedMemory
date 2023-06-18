//
// Created by michi on 6/18/23.
//

#ifndef SHAREDMEMORY_SHAREDFUNCTIONS_H
#define SHAREDMEMORY_SHAREDFUNCTIONS_H

#endif //SHAREDMEMORY_SHAREDFUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

short ringbuffer_init(ring_buffer * buf, size_t bufferSize);

void cleanup(int shmId, void *shmAddr);