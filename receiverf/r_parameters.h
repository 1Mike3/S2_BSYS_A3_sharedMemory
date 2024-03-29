//
// Created by michi on 5/28/23.
//

#ifndef SHAREDMEMORY_R_PARAMETERS_H
#define SHAREDMEMORY_R_PARAMETERS_H

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

//Function for managing the parameters of the receiverf

/**
 *
 * @param argc ...
 * @param argv ...
 * @param byteSize The size of the buffer in bytes
 * @return -1 failed , 0 success
 */

short manageParameters(int argc, char *argv[], size_t *byteSize);

#endif //SHAREDMEMORY_R_PARAMETERS_H
