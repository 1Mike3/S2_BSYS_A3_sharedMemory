//
// Created by michi on 5/28/23.
//

#include "r_memoryManagement.h"
#include <signal.h>
#include "stdbool.h"

bool SIGINT_Received = false;
void signalHandlerSigint_Read(int sig){
#if DEBUG_SIG
    fprintf(stderr,"\n[R] Signal Handler: SIGINT received\n");
#endif
    SIGINT_Received = true;
}
//declaation of the signal handler from main so it can be called after read is finished when SIGINT is received
extern void sigint_handler(int sig);

#pragma region Creating Shared Memory
short create_shared_memory(key_t * key,const size_t * bufferSize,int * shmid_sharedMemoryID,void ** shmaddr_sharedMemoryAddress){

    if(*key == -1){
        fprintf(stderr, "[R] Error in ftok\n");
        perror("[R] PError in ftok: ");
        return -1;
    }


// generate the shared memory identifier
    *shmid_sharedMemoryID = shmget(*key, *bufferSize, 0644);
    if(*shmid_sharedMemoryID == -1 ){
        fprintf(stderr,"[R] Error in shmget\n");
        perror("[R] Perror in shmget: ");
        return -1;
    }

//attach the shared memory
    *shmaddr_sharedMemoryAddress = shmat(*shmid_sharedMemoryID, NULL,0 );
    if(*shmaddr_sharedMemoryAddress == (void *) -1  || *shmaddr_sharedMemoryAddress == NULL){
        fprintf(stderr,"[R] Error in shmat\n");
        perror("[R] Perror in shmat: ");
        return -1;
    }

    return 0;
}
#pragma endregion Creating Shared Memory




//function to read from the ringbuffer and write to stdout
#pragma region read from ringbuffer
short read_from_ringbuffer(ring_buffer * buf, size_t readBufferSize){


    struct sigaction sa;
    sa.sa_handler = &signalHandlerSigint_Read;
    sa.sa_flags = SA_RESTART;   // Restart functions if interrupted by handler
    sigaction(SIGINT, &sa, NULL); // Install handler

    //char * readBuffer = calloc(readBufferSize ,sizeof(char));
    sem_wait(buf->sem_ptr);

    while(SIGINT_Received == false || buf->emptyStateBool == false) {  //while loop for continous reading
        //will be interrupted by the signal handler will shutdown after finished reading


        //check if the readBuffer is big enough
        if (readBufferSize != buf->buffer_size) {
            fprintf(stderr, "\n[R] The requested Buffer Value does not match the allready allocated Value\n");
            fprintf(stderr, "[R] The Current Buffer Value is: %lu\n\n", buf->buffer_size);
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
    } // End read while loop

    sem_post(buf->sem_ptr);
    sigint_handler(SIGINT); //call the standard signal handler from main to exit the program

    return 0;
}
#pragma endregion read from ringbuffer





