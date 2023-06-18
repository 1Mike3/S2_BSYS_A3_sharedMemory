//
// Created by michi on 5/28/23.
//

#ifndef SHAREDMEMORY_DEFINITIONS_H
#define SHAREDMEMORY_DEFINITIONS_H
#include <stdbool.h>
#include <nl_types.h>
#include <sys/types.h>
#include <stdlib.h>
#include <semaphore.h>

#define DEBUG 0
#define DEBUG_SIG 1
#define SEM_NAME "/RICHARD" //name of the semaphore

// This is the ring buffer structure
// It contains the head and tail pointers
// It also contains the buffer itself

typedef struct ring_buffer {
    int head; //head is the next byte to read
    int tail;  //tail is the next free position
    char * buffer; //the actual buffer
    size_t buffer_size; //size of the buffer in bytes
    //name of the semaphore can be found above in definitions.h
    sem_t sem; //semaphore for mutual exclusion
    sem_t * sem_ptr; //pointer to the semaphore
    pid_t pid_sender;  //the rescpective pids of the processes to enable communtication between them
    pid_t pid_receiver;
    bool overflowStateBool; //a indicator signaling that the buffer is full
    bool emptyStateBool; //a indicator signaling that the buffer is empty
    bool sem_Init_Bool; //a indicator signaling that the semaphore is initialized
} ring_buffer;




//SHARED MEMORY ... obsolete
#define SHM_KEY 0x5555 //key for the shared memory

#endif //SHAREDMEMORY_DEFINITIONS_H
