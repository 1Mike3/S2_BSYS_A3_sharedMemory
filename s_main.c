
#include "s_parameters.h"
#include "definitions.h"
#include "s_memoryManagement.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "sharedFunctions.h"




//I am saving the memory addresses to be freed in the event of a Signal here for easyier access
//I know that global Variables are bad, but in this case it is saving me a lot of hassle and not causing any problems

//Adresses SM
//Struct
int shmid_sharedMemoryID_0 = -1;
void * shmaddr_sharedMemoryAddress_0 = NULL;
//Shared Memory Buffer
int shmid_sharedMemoryID_1 = -1;
void * shmaddr_sharedMemoryAddress_1 = NULL;
//not the actual buffer , just a second reference for cleanup
ring_buffer * buf_cpy = NULL;

void sigint_handler(int sig){
    FILE * file = fopen("S_Logs.txt", "a");
    fprintf(file, "Received SIGINT, freeing and shutting down, PID = %i\n", getpid());
    fclose(file);

    if(buf_cpy != NULL)
        cleanupSemaphore(buf_cpy);

    cleanup(shmid_sharedMemoryID_0, shmaddr_sharedMemoryAddress_0);
    cleanup(shmid_sharedMemoryID_1, shmaddr_sharedMemoryAddress_1);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
#if DEBUG
    printf("[S] Hello i am the sender, my PID is : %i\n\n", getpid());
#endif
    FILE * f =fopen("S_PID.txt", "w");
    fprintf(f, "%i", getpid());
    fclose(f);

#pragma region Variables
//Manage Parameters

//Signals
    struct sigaction sa;
    sa.sa_handler = &sigint_handler;
    sa.sa_flags = SA_RESTART;   // Restart functions if interrupted by handler
    sigaction(SIGINT, &sa, NULL); // Install handler

    size_t bufferSize = 0;
    //Pointer to the data

    //key generating
    key_t key_0 = ftok("../shared/keyGen", 'R');
    key_t key_1 = ftok("../shared/keyGen2", 'R');


    //Error Case ftok
    if(key_0 == -1 || key_1 == -1){
        printf("[S] Error in ftok\n");
        printf("[S] Program Shutting Down!\n");
        exit(EXIT_FAILURE);
    }
#pragma endregion Variables


#pragma region Parameter Management
    //manage the parameters

    if(-1 == manageParameters(argc, argv, &bufferSize)){
        printf("[S] Error in manageParameters\n");
        printf("[S] Program Shutting Down!\n");
        exit(EXIT_FAILURE);
    }
    if(bufferSize == 0){
        printf("[S] Buffer size is 0\n");
        printf("[S] Correct Usage: senderf -m [Desired Byte-size > 0]\n");
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
    cleanup(shmid_sharedMemoryID_0, shmaddr_sharedMemoryAddress_0);
}

//shared memory - Buffer

    short retVal_create_Shared_1 = create_shared_memory(&key_1, &bufferSize, &shmid_sharedMemoryID_1, &shmaddr_sharedMemoryAddress_1);
    if(retVal_create_Shared_1 == -1) {
        printf("[S] Error in create_shared_memory\n");
        printf("[S] Program Shutting Down!\n");
        goto cleanupLabel;

}
#pragma endregion Creating Shared Memory



#pragma region setting up the ringbuffer
//setting up the ringbuffer
ring_buffer * ringBuffer;
ringBuffer = shmaddr_sharedMemoryAddress_0;
ringBuffer->buffer = shmaddr_sharedMemoryAddress_1;

//for cleanup
buf_cpy = ringBuffer;

#pragma endregion setting up the ringbuffer



#pragma region init ringbuffer
    ringbuffer_init(ringBuffer, bufferSize);
    ringBuffer->pid_sender = getpid();
#if DEBUG
    printf("[S] Ringbuffer initialized\n");
#endif
#pragma endregion init ringbuffer


#pragma region write to buffer


    short retVal_ringbufferWrite = ringbuffer_write(ringBuffer);
if(retVal_ringbufferWrite == -1){
    printf("[S] Error in ringbuffer_write\n");
    printf("[S] Program Shutting Down!\n");
    goto cleanupLabel;
}
    //Old Testwrite to Buffer
    /*
//writing some random stuff into the buffer and seeing if i can get it out
    char * testString = "This information has been written into the buffer";
    for(int i = 0; i < 50; i++){
        ringBuffer->buffer[i] = testString[i];
    }
    printf("[S] Wrote %s into the buffer\n\n", ringBuffer->buffer);
     */
#pragma endregion write to buffer



//Debug BlockS
#if DEBUG
    printf("Sender Debug\n\n");
    //printing the shmid
    printf("[S] shmid_sharedMemoryID_0: %d\n", shmid_sharedMemoryID_0);
    printf("[S] shmid_sharedMemoryID_1: %d\n", shmid_sharedMemoryID_1);
    printf("[S] key_0: %d\n", key_0);
    printf("[S] key_1: %d\n", key_1);
    printf("[S] shmaddr_sharedMemoryAddress_0: %p\n", shmaddr_sharedMemoryAddress_0);
    printf("[S] shmaddr_sharedMemoryAddress_1: %p\n\n", shmaddr_sharedMemoryAddress_1);

    shmdt(shmaddr_sharedMemoryAddress_0);
    shmdt(shmaddr_sharedMemoryAddress_1);
#endif

    //OLD
/*

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



#pragma endregion cleanup
*/

    return 0;

    cleanupLabel:

    cleanup(shmid_sharedMemoryID_0, shmaddr_sharedMemoryAddress_0);
    cleanup(shmid_sharedMemoryID_1, shmaddr_sharedMemoryAddress_1);
    exit(EXIT_FAILURE);

}

//todo dont forget to eventually free the calloc from the write fucntion