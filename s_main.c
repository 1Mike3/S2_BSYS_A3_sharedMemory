
#include "s_parameters.h"
#include "definitions.h"
#include "s_memoryManagement.h"
#include <stdio.h>
#include <stdlib.h>
 #include <sys/types.h>
 #include <sys/ipc.h>
 #include <sys/shm.h>
#include <errno.h>

#define DEBUG 1

int main(int argc, char *argv[]){

//Variables
//Manage Parameters
    size_t bufferSize = 0;



    //shmat :: attaching the shared memory
    void * shmaddr_sharedMemoryAddress = NULL;



#pragma region Parameter Management
    //manage the parameters

    if(-1 == manageParameters(argc, argv, &bufferSize)){
        printf("[S] Error in manageParameters\n");
        printf("[S] Program Shutting Down!\n");
        exit(EXIT_FAILURE);
    }
    if(bufferSize == 0){
        printf("[S] Buffer size is 0\n");
        printf("[S] Correct Usage: sender -m [Desired Byte-size > 0]\n");
        printf("[S] Program Shutting Down!\n");
        exit(EXIT_FAILURE);
    }

#pragma endregion Parameter Management

#pragma region Creating Ring Buffer
    ring_buffer * ringBuffer = createRingBuffer(bufferSize);
    if(ringBuffer == NULL){
        printf("[S] Error in createRingBuffer\n");
        printf("[S] Program Shutting Down!\n");
        exit(EXIT_FAILURE);
    }
#pragma endregion Creating Ring Buffer

#pragma region Creating Shared Memory

    //generating key
    key_t key = ftok("./keyGen", 'R');



        //VARS shmget :: getting the shared memory


        //return Value
        int shmid_sharedMemoryID;

// generate the shared memory identifier
    shmid_sharedMemoryID = shmget(key, bufferSize, 0644 | IPC_CREAT);
    if(shmid_sharedMemoryID == -1){
        printf("[S] Error in shmget\n");
        perror("[S] PError in shmget: ");
        printf("[S] Program Shutting Down!\n");
        exit(EXIT_FAILURE);
    }

//attach the shared memory
    shmaddr_sharedMemoryAddress = shmat(shmid_sharedMemoryID, NULL,0 );
    if(shmaddr_sharedMemoryAddress == (void *) -1){
        printf("[S] Error in shmat\n");
        perror("[S] Perror in shmat: ");
        printf("[S] Program Shutting Down!\n");
        exit(EXIT_FAILURE);
    }
    cleanup(shmid_sharedMemoryID, shmaddr_sharedMemoryAddress);
    //ringBuffer->buffer = shmaddr_sharedMemoryAddress; //casting t
    // the address to the ringbuffer




#pragma region debug write test
//writing some random stuff into the buffer and seeing if i can get it out
#pragma endregion debug write test

#pragma region debug detch and



    //print all the ringbuffer information in an #ifdef DEBUG block
#pragma region debug block Print buffer
#ifdef DEBUG
    printf("[S] Ringbuffer information:\n");
    printf("[S] Head: %d\n", ringBuffer->head);
    printf("[S] Tail: %d\n", ringBuffer->tail);
    printf("[S] Buffer: %p\n", ringBuffer->buffer);
    printf("[S] Buffer size: %zu\n", ringBuffer->buffer_size);
    printf("[S] Overflow: %d\n", ringBuffer->overflowStateBool);
    printf("[S] Empty: %d\n", ringBuffer->emptyStateBool);

#endif
#pragma endregion DEBUG



    return 0;
}
