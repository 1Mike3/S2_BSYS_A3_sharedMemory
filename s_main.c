
#include "s_parameters.h"
#include "definitions.h"
#include "s_memoryManagement.h"
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 1

int main(int argc, char *argv[]){


size_t bufferSize = 0;
    if(-1 == manageParameters(argc, argv, &bufferSize)){
        printf("[S] Error in manageParameters\n");
        return -1;
    }

    ring_buffer * ringBuffer = createRingBuffer(bufferSize);

    //print all the ringbuffer information in an #ifdef DEBUG block
    #ifdef DEBUG
    printf("[S] Ringbuffer information:\n");
    printf("[S] Head: %d\n", ringBuffer->head);
    printf("[S] Tail: %d\n", ringBuffer->tail);
    printf("[S] Buffer: %p\n", ringBuffer->buffer);
    printf("[S] Buffer size: %zu\n", ringBuffer->buffer_size);
    printf("[S] Overflow: %d\n", ringBuffer->overflow);
    printf("[S] Empty: %d\n", ringBuffer->empty);
    #endif


    return 0;
}
