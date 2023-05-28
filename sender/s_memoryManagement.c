//
// Created by michi on 5/28/23.
//

#include "s_memoryManagement.h"
#include "../shared/definitions.h"

ring_buffer * createRingBuffer(size_t bufferSize){
    ring_buffer * buf = malloc(sizeof(ring_buffer));
    buf->buffer = calloc(bufferSize, sizeof(char));
    buf->buffer_size = bufferSize;
    buf->head = 0;
    buf->tail = 0;
    buf->overflow = false;
    buf->empty = true;
    return buf;
}
