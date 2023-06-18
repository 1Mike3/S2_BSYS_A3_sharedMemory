//
// Created by michi on 5/28/23.
//

#include "s_memoryManagement.h"
#include "../shared/definitions.h"

#include <unistd.h>




#pragma region Creating Shared Memory
short create_shared_memory(key_t * key,const size_t * bufferSize,int * shmid_sharedMemoryID,void ** shmaddr_sharedMemoryAddress){




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



#pragma region write to Ring Buffer



short ringbuffer_write(ring_buffer * buf){

//TODO free calloc stdinBuf
    char * stdInBuf= calloc(buf->buffer_size,sizeof(char));
    char * retVal_fgets;
    char  workingCharPointer = '\0';




                retVal_fgets = fgets(stdInBuf, (int) buf->buffer_size, stdin);

                /*
                if (retVal_fgets == NULL) {
                    printf("[S] Error in fgets\n");
                    perror("[S] PError in fgets: ");
                    return -1;
                }
        */

                workingCharPointer = stdInBuf[0];


                while (workingCharPointer != EOF && workingCharPointer != '\0' ) {




                    //check if buffer is full
                    if (buf->head == buf->tail && buf->overflowStateBool == true) {
                        printf("[S] Buffer is full\n");
                        return -1;
                    }

                    //check if buffer is empty
                    if (buf->head == buf->tail && buf->emptyStateBool == true) {
                        buf->emptyStateBool = false;
                    }

                    //write to buffer
                    buf->buffer[buf->head] = workingCharPointer;
                    buf->head = (buf->head + 1) %
                                (int) buf->buffer_size; //Headpointer loops back to 0 if it reaches the end of the buffer


                    if (buf->head == buf->tail) {
                        buf->overflowStateBool = true;
                    }

                    workingCharPointer = stdInBuf[buf->head];
                }

                buf->buffer[buf->head] = workingCharPointer;



  return 0;
}




#pragma endregion write to Ring Buffer






