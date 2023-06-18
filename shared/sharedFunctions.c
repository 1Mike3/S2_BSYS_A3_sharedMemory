//
// Created by michi on 6/18/23.
//

#include "sharedFunctions.h"


#pragma region init Ring Buffer


short ringbuffer_init(ring_buffer * buf, size_t bufferSize){

    buf->buffer[0] = '\0';
    buf->buffer_size = bufferSize;
    buf->head = 0;
    buf->tail = 0;
    buf->overflowStateBool = false;
    buf->emptyStateBool = true;
    buf->sem_empty = NULL;
    buf->sem_full = NULL;
    buf->mutex = NULL;
    buf->pid_receiver = 0;
    buf->pid_sender = 0;
    return 0;
}

#pragma endregion init Ring Buffer


#pragma region cleanup
void cleanup(int shmId, void *shmAddr) {
    // Detach from the shared memory segment
    if(shmAddr != NULL){ //only detach if the address is actually set
        if (shmdt(shmAddr) == -1) {
            perror("[R] shmdt failed");
            exit(EXIT_FAILURE);
        }
    }
    // Mark the segment to be destroyed
    if(shmId != -1) { //only delete if the id is not initial Value
        if (shmctl(shmId, IPC_RMID, NULL) == -1) {
            perror("[R] shmctl failed");
            exit(EXIT_FAILURE);
        }
    }
}

#pragma endregion cleanup