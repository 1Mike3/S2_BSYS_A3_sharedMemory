
#include "s_parameters.h"
#include "definitions.h"
#include "s_memoryManagement.h"
#include <stdio.h>
#include <stdlib.h>


#define DEBUG 1

int main(int argc, char *argv[]){

//Variables
//Manage Parameters
    size_t bufferSize = 0;

//Shared Memory
    int shmid_sharedMemoryID;
    key_t key;
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

   short retVal_create_Shared = create_shared_memory(&key, &bufferSize, &shmid_sharedMemoryID, &shmaddr_sharedMemoryAddress);
if(retVal_create_Shared == -1){
    printf("[S] Error in create_shared_memory\n");
    printf("[S] Program Shutting Down!\n");
    exit(EXIT_FAILURE);
}
#pragma endregion Creating Shared Memory

//Test Write something into the buffer
ringBuffer->buffer = shmaddr_sharedMemoryAddress;

#pragma region debug write test
//writing some random stuff into the buffer and seeing if i can get it out
    char * testString = "Hello World";
    for(int i = 0; i < 11; i++){
        ringBuffer->buffer[i] = testString[i];
    }
    printf("[S] Wrote %s into the buffer\n", ringBuffer->buffer);
#pragma endregion debug write test


//print all the ringbuffer information in an #ifdef DEBUG block
#pragma region debug block Print buffer
#ifdef DEBUG
    printf("[S] Ringbuffer information:\n");
    printf("[S] Head: %d\n", ringBuffer->head);
    printf("[S] Tail: %d\n", ringBuffer->tail);
    printf("[S] Buffer: %p\n", ringBuffer->buffer);
    printf("[S] Buffer size: %zu\n", ringBuffer->buffer_size);
    printf("[S] Overflow: %d\n", ringBuffer->overflowStateBool);
    printf("[S] Empty: %d\n\n", ringBuffer->emptyStateBool);

    printf("[S] Memory Print:\n");
    for(int i = 0; i < ringBuffer->buffer_size; i++){
        printf("%c", ringBuffer->buffer[i]);
        if(i%10 == 0 && i != 0){
            printf("\n");
        }
    }

    printf("\n\n");

#endif
#pragma endregion DEBUG


#pragma region cleanup
    cleanup(shmid_sharedMemoryID, shmaddr_sharedMemoryAddress);
#pragma endregion cleanup

    return 0;
}
