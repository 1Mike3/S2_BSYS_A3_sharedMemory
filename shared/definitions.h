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

// This is the ring buffer structure
// It contains the head and tail pointers
// It also contains the buffer itself

//Note as far as i could understand it from the assignment there is no need for the buffer to loop around, so
//i will not implement it that way

//TODO write all relevant ring buffer information into this struct so i can easieer share it between the processes and functions
typedef struct ring_buffer {
    int head; //head is the next byte to read
    int tail;  //tail is the next free position
    char * buffer; //the actual buffer
    size_t buffer_size; //size of the buffer in bytes
    sem_t * sem_full; //semaphore containing the number of full bytes
    sem_t * sem_empty; // semaphore containing the number of empty bytes
    pthread_mutex_t * mutex; //Mutex for the mutual exclusion of the two processes
    bool overflowStateBool; //a indicator signaling that the buffer is full
    bool emptyStateBool; //a indicator signaling that the buffer is empty
} ring_buffer;

//SHARED MEMORY
#define SHM_KEY 0x5555 //key for the shared memory

#endif //SHAREDMEMORY_DEFINITIONS_H
