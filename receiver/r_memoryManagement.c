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






#pragma region cleanup
void cleanup(int shmId, void *shmAddr) {
    // Detach from the shared memory segment
    if (shmdt(shmAddr) == -1) {
        perror("[R] shmdt failed");
        // handle the error...
        exit(EXIT_FAILURE);
    }

    // Mark the segment to be destroyed
    if (shmctl(shmId, IPC_RMID, NULL) == -1) {
        perror("[R] shmctl failed");
        exit(EXIT_FAILURE);
    }
}
#pragma endregion cleanup