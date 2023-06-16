//
// Created by michi on 5/28/23.
//

#include "s_memoryManagement.h"
#include "../shared/definitions.h"





#pragma region Creating Shared Memory
short create_shared_memory(key_t * key,const size_t * bufferSize,int * shmid_sharedMemoryID,void ** shmaddr_sharedMemoryAddress){

if(*key == -1){
    printf("[S] Error in ftok\n");
    perror("[S] PError in ftok: ");
    return -1;
}


// generate the shared memory identifier
    *shmid_sharedMemoryID = shmget(*key, *bufferSize, 0644 | IPC_CREAT);
    if(*shmid_sharedMemoryID == -1){
        printf("[S] Error in shmget\n");
        perror("[S] PError in shmget: ");
        return -1;
    }

//attach the shared memory
    *shmaddr_sharedMemoryAddress = shmat(*shmid_sharedMemoryID, NULL,0 );
    if(*shmaddr_sharedMemoryAddress == (void *) -1  || *shmaddr_sharedMemoryAddress == NULL){
        printf("[S] Error in shmat\n");
        perror("[S] Perror in shmat: ");
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

#pragma region write to Ring Buffer



short ringbuffer_write(ring_buffer * buf,char * data){

    char * workingCharPointer = data;

    while ( (*(workingCharPointer +buf->head) != EOF) && (*(workingCharPointer +buf->head) != '\0') ) {

        //check if buffer is full
        if(buf->head == buf->tail && buf->overflowStateBool == true){
            printf("[S] Buffer is full\n");
            return -1;
        }

        //check if buffer is empty
        if(buf->head == buf->tail && buf->emptyStateBool == true){
            buf->emptyStateBool = false;
        }

        //write to buffer
        buf->buffer[buf->head] = *(workingCharPointer +buf->head);
        buf->head = (buf->head + 1) % (int)buf->buffer_size; //Headpointer loops back to 0 if it reaches the end of the buffer


        if(buf->head == buf->tail){
            buf->overflowStateBool = true;
        }

    }

  return 0;
}


#pragma endregion write to Ring Buffer


#pragma region cleanup
void cleanup(int shmId, void *shmAddr) {
    // Detach from the shared memory segment
    if (shmdt(shmAddr) == -1) {
        perror("[S] shmdt failed");
        // handle the error...
        exit(EXIT_FAILURE);
    }

    // Mark the segment to be destroyed
    if (shmctl(shmId, IPC_RMID, NULL) == -1) {
        perror("[S] shmctl failed");
        exit(EXIT_FAILURE);
    }
}
#pragma endregion cleanup


