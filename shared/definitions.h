//
// Created by michi on 5/28/23.
//

#ifndef SHAREDMEMORY_DEFINITIONS_H
#define SHAREDMEMORY_DEFINITIONS_H
#include <stdbool.h>
#include <nl_types.h>
#include <sys/types.h>
#include <stdlib.h>

// This is the ring buffer structure
// It contains the head and tail pointers
// It also contains the buffer itself
//write all relevant ring buffer information into this struct
typedef struct ring_buffer {
    int head;
    int tail;
    char * buffer;
    size_t buffer_size;
    bool overflow;
    bool empty;
} ring_buffer;

// This is the shared memory structure
// It contains the ring buffer and the catalog descriptor
//write all relevant shared memory information into this struct
typedef struct shared_memory {
    ring_buffer * ring_buffer;
    nl_catd catd;
} shared_memory;

#endif //SHAREDMEMORY_DEFINITIONS_H
