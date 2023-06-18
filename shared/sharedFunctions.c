//
// Created by michi on 6/18/23.
//

//#include "definitions.h"
#include "fcntl.h"
#include "sharedFunctions.h"

#include <semaphore.h>



#pragma region init Ring Buffer


short ringbuffer_init(ring_buffer * buf, size_t bufferSize){

    buf->buffer[0] = '\0';
    buf->buffer_size = bufferSize;
    buf->head = 0;
    buf->tail = 0;
    buf->overflowStateBool = false;
    buf->emptyStateBool = true;
    buf->sem;
    sem_init(&(buf->sem), 0, 1); //init semaphore
    buf->sem_ptr = sem_open(SEM_NAME, O_CREAT, 0644, 1); //open semaphore
    buf->pid_receiver = 0;
    buf->pid_sender = 0;
    buf->sem_Init_Bool = true;
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

void cleanupSemaphore(ring_buffer * buf){
    if(buf->sem_Init_Bool == false){
        return;
    } else {
      int retCL =  sem_close(buf->sem_ptr);
        if(retCL == -1){
            perror("[R] sem_close failed");
            exit(EXIT_FAILURE);
        }
       int retUL = sem_unlink(SEM_NAME);
        if(retUL == -1){
            perror("[R] sem_unlink failed");
            exit(EXIT_FAILURE);
        }
        buf->sem_Init_Bool = false;
    }
}

#pragma endregion cleanup