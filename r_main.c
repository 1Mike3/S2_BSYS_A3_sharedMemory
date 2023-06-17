//
// Created by michi on 5/26/23.
//
#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"

#include "r_parameters.h"
#include "r_memoryManagement.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define DEBUG 1

//I am saving the memory addresses to be freed in the event of a Signal here for easyier access
//I know that global Variables are bad, but in this case it is saving me a lot of hassle and not causing any problems

//Adresses SM
//Struct
int shmid_sharedMemoryID_0 = -1;
void * shmaddr_sharedMemoryAddress_0 = NULL;
//Shared Memory Buffer
int shmid_sharedMemoryID_1 = -1;
void * shmaddr_sharedMemoryAddress_1 = NULL;


void sigint_handler(int sig) {
    printf("[R] Received SIGINT, shutting down...\n");
    cleanup(shmid_sharedMemoryID_0, shmaddr_sharedMemoryAddress_0);
    cleanup(shmid_sharedMemoryID_1, shmaddr_sharedMemoryAddress_1);
    exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[]) {
#if DEBUG
printf("[R] Hello i am the receiver, my PID is : %i\n\n", getpid());
FILE * f =fopen("R_PID.txt", "w");
fprintf(f, "%i", getpid());
fclose(f);
#endif

#pragma region Variables
//Variables

//signals
struct sigaction sa;
sa.sa_handler = &sigint_handler;
sa.sa_flags = SA_RESTART;   // Restart functions if interrupted by handler
sigaction(SIGINT, &sa, NULL); // Install handler

//Manage Parameters
    size_t bufferSize = 0;

 //key generating
key_t key_0 = ftok("../shared/keyGen", 'R');
key_t key_1 = ftok("../shared/keyGen2", 'R');

    //Error Case ftok
    if(key_0 == -1 || key_1 == -1){
        printf("[R] Error in ftok\n");
        printf("[R] Program Shutting Down!\n");
        exit(EXIT_FAILURE);
    }
#pragma endregion Variables

#pragma region Parameter Management
    if(-1 == manageParameters(argc, argv, &bufferSize)){
        printf("[R] Error in manageParameters\n");
        printf("[R] Program Shutting Down!\n");
        exit(EXIT_FAILURE);
    }
    if(bufferSize == 0){
        printf("[R] Buffer size is 0\n");
        printf("[R] Correct Usage: receiver -m [Desired Byte-size > 0]\n");
        printf("[R] Program Shutting Down!\n");
        exit(EXIT_FAILURE);
    }
#pragma endregion Parameter Management



#pragma region Creating Shared Memory
//shared memory - structRingbuffer
    size_t structSize_0 = sizeof(ring_buffer);
    short retVal_create_Shared_0 = create_shared_memory(&key_0, &structSize_0, &shmid_sharedMemoryID_0, &shmaddr_sharedMemoryAddress_0);
    if(retVal_create_Shared_0 == -1) {
        printf("[R] Error in create_shared_memory\n");
        printf("[R] Program Shutting Down!\n");
        exit(EXIT_FAILURE);
    }

//shared memory - Buffer
    short retVal_create_Shared_1 = create_shared_memory(&key_1, &bufferSize, &shmid_sharedMemoryID_1, &shmaddr_sharedMemoryAddress_1);
    if(retVal_create_Shared_1 == -1) {
        printf("[R] Error in create_shared_memory\n");
        printf("[R] Program Shutting Down!\n");
        goto cleanupLabel;
    }
#pragma endregion Creating Shared Memory



#pragma region create Ring Buffer
    ring_buffer * ringBuffer;
    ringBuffer = shmaddr_sharedMemoryAddress_0;
    ringBuffer->buffer = shmaddr_sharedMemoryAddress_1;
#pragma endregion create Ring Buffer




    printf("Receiver Debug\n\n");
    //Receiver Debug shmid
    printf("[R] shmid_sharedMemoryID_0: %d\n", shmid_sharedMemoryID_0);
    printf("[R] shmid_sharedMemoryID_1: %d\n", shmid_sharedMemoryID_1);
    printf("[R] key_0: %d\n", key_0);
    printf("[R] key_1: %d\n", key_1);
    printf("[R] shmaddr_sharedMemoryAddress_0: %p\n", shmaddr_sharedMemoryAddress_0);
    printf("[R] shmaddr_sharedMemoryAddress_1: %p\n\n", shmaddr_sharedMemoryAddress_1);




    //print all the ringbuffer information in an #ifdef DEBUG block
#pragma region debug block Print buffer
#if DEBUG
    printf("[R] Ringbuffer information:\n");
    printf("[R] Head: %d\n", ringBuffer->head);
    printf("[R] Tail: %d\n", ringBuffer->tail);
    printf("[R] Buffer: %p\n", ringBuffer->buffer);
    printf("[R] Buffer size: %zu\n", ringBuffer->buffer_size);
    printf("[R] Overflow: %d\n", ringBuffer->overflowStateBool);
    printf("[R] Empty: %d\n\n", ringBuffer->emptyStateBool);

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

    short retVal_read_f_ringbuf = read_from_ringbuffer(ringBuffer, bufferSize);
    if(retVal_read_f_ringbuf == -1){
        printf("[R] Error in read_from_ringbuffer\n");
        printf("[R] Program Shutting Down!\n");
        goto cleanupLabel;
    }



#pragma region cleanup
//regular cleanup performed on exit
    cleanup(shmid_sharedMemoryID_0, shmaddr_sharedMemoryAddress_0);
    cleanup(shmid_sharedMemoryID_1, shmaddr_sharedMemoryAddress_1);

#pragma endregion cleanup


    return 0;

    //Error Case goto, irregular cleanup performed on exit after a failure
    cleanupLabel:

    cleanup(shmid_sharedMemoryID_0, shmaddr_sharedMemoryAddress_0);
    cleanup(shmid_sharedMemoryID_1, shmaddr_sharedMemoryAddress_1);
    exit(EXIT_FAILURE);
}

//TODO add check so the reciever waits for the initializing of the ringbuffer until the sender has spawned


//TODO maybe bug if when writing data in parameter function not + 1 strncpy size