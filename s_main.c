
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

//Shared Memory ringbufferStruct
    int shmid_sharedMemoryID_0;
    key_t key_0 = ftok("../shared/keyGen", 'R');
    void * shmaddr_sharedMemoryAddress_0 = NULL;
    //Shared Memory Buffer
    int shmid_sharedMemoryID_1;
    key_t key_1 = ftok("../shared/keyGen2", 'R');
    void * shmaddr_sharedMemoryAddress_1 = NULL;


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



#pragma region Creating Shared Memory

//shared memory - structRingbuffer

    size_t structSize_0 = sizeof(ring_buffer);

   short retVal_create_Shared_0 = create_shared_memory(&key_0, &structSize_0, &shmid_sharedMemoryID_0, &shmaddr_sharedMemoryAddress_0);
if(retVal_create_Shared_0 == -1) {
    printf("[S] Error in create_shared_memory\n");
    printf("[S] Program Shutting Down!\n");
    exit(EXIT_FAILURE);
}

//shared memory - Buffer

    short retVal_create_Shared_1 = create_shared_memory(&key_1, &bufferSize, &shmid_sharedMemoryID_1, &shmaddr_sharedMemoryAddress_1);
    if(retVal_create_Shared_1 == -1) {
        printf("[S] Error in create_shared_memory\n");
        printf("[S] Program Shutting Down!\n");
        exit(EXIT_FAILURE);
}
#pragma endregion Creating Shared Memory





ring_buffer * ringBuffer;
ringBuffer = shmaddr_sharedMemoryAddress_0;
ringBuffer->buffer = shmaddr_sharedMemoryAddress_1;




#pragma region debug write test
//writing some random stuff into the buffer and seeing if i can get it out
    char * testString = "Hello World";
    for(int i = 0; i < 11; i++){
        ringBuffer->buffer[i] = testString[i];
    }
    printf("[S] Wrote %s into the buffer\n\n", ringBuffer->buffer);
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
    for(int i = 0; i < bufferSize; i++){
        printf("%c", ringBuffer->buffer[i]);
        if(i%10 == 0 && i != 0){
            printf("\n");
        }
    }

    printf("\n\n");

#endif
#pragma endregion DEBUG


#pragma region cleanup

    cleanup(shmid_sharedMemoryID_0, shmaddr_sharedMemoryAddress_0);
    cleanup(shmid_sharedMemoryID_1, shmaddr_sharedMemoryAddress_1);

#pragma endregion cleanup

    return 0;
}
