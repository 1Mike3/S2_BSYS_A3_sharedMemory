//
// Created by michi on 5/26/23.
//
#include <stdio.h>
#include <stdlib.h>

#include "r_parameters.h"

int main(int argc, char *argv[]) {



    size_t bufferSize = 0;
    if(-1 == manageParameters(argc, argv, &bufferSize)){
        printf("[R] Error in manageParameters\n");
        return -1;
    }

    return 0;
}
