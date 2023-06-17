//
// Created by michi on 5/28/23.
//

#include "r_memoryManagement.h"



#pragma region Creating Shared Memory
short create_shared_memory(key_t * key,const size_t * bufferSize,int * shmid_sharedMemoryID,void ** shmaddr_sharedMemoryAddress){

    if(*key == -1){
        printf("[R] Error in ftok\n");
        perror("[R] PError in ftok: ");
        return -1;
    }


// generate the shared memory identifier
    *shmid_sharedMemoryID = shmget(*key, *bufferSize, 0644);
    if(*shmid_sharedMemoryID == -1 ){
        printf("[R] Error in shmget\n");
        perror("[R] Perror in shmget: ");
        return -1;
    }

//attach the shared memory
    *shmaddr_sharedMemoryAddress = shmat(*shmid_sharedMemoryID, NULL,0 );
    if(*shmaddr_sharedMemoryAddress == (void *) -1  || *shmaddr_sharedMemoryAddress == NULL){
        printf("[R] Error in shmat\n");
        perror("[R] Perror in shmat: ");
        return -1;
    }

    return 0;
}
#pragma endregion Creating Shared Memory


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
    return 0;
}

#pragma endregion init Ring Buffer

//function to read from the ringbuffer and write to stdout
#pragma region read from ringbuffer
short read_from_ringbuffer(ring_buffer * buf, size_t readBufferSize){

    //char * readBuffer = calloc(readBufferSize ,sizeof(char));
    while(1) {  //while loop for continous reading


        //check if the readBuffer is big enough
        if (readBufferSize != buf->buffer_size) {
            printf("\n[R] The requested Buffer Value does not match the allready allocated Value\n");
            printf("[R] The Current Buffer Value is: %lu\n\n", buf->buffer_size);
           // free(readBuffer);
            return -1;
        }

        //read from the ringbuffer
        while (buf->head != buf->tail){
            buf->emptyStateBool = false;
            fputc(buf->buffer[buf->tail], stdout);
            buf->tail = (buf->tail + 1) % (int)buf->buffer_size;
        }
        //set the ringbuffer to empty
        buf->emptyStateBool = true;
    }

    return 0;
}
#pragma endregion read from ringbuffer





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