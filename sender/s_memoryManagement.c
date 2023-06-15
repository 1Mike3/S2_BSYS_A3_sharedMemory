//
// Created by michi on 5/28/23.
//

#include "s_memoryManagement.h"
#include "../shared/definitions.h"


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


#pragma region Creating Shared Memory
short create_shared_memory(key_t * key,const size_t * bufferSize,int * shmid_sharedMemoryID,void ** shmaddr_sharedMemoryAddress){
    //generating key
    *key = ftok("./keyGen", 'R');
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
    if(*shmaddr_sharedMemoryAddress == (void *) -1){
        printf("[S] Error in shmat\n");
        perror("[S] Perror in shmat: ");
        return -1;
    }

    return 0;
}



#pragma endregion Creating Shared Memory


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


