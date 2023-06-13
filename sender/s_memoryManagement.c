//
// Created by michi on 5/28/23.
//

#include "s_memoryManagement.h"
#include "../shared/definitions.h"
#include <fcntl.h>
#include <sys/stat.h>



#pragma region Creating Ring Buffer

ring_buffer * createRingBuffer(size_t bufferSize){
    ring_buffer * buf = malloc(sizeof(ring_buffer));
    buf->buffer = calloc(bufferSize, sizeof(char));
    buf->buffer_size = bufferSize;
    buf->head = 0;
    buf->tail = 0;
    buf->overflowStateBool = false;
    buf->emptyStateBool = true;
    buf->sem_empty = NULL;
    buf->sem_full = NULL;
    return buf;
}

#pragma endregion Creating Ring Buffer




