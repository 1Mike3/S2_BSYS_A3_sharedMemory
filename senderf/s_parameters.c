//
// Created by michi on 5/28/23.
//

#include "s_parameters.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include "definitions.h"


short manageParameters(int argc, char *argv[], size_t *byteSize) {



    //get opt inmplementation to let the user enter bytesize with the -m flag
    int opt;

    while ((opt = getopt(argc, argv, "m:")) != -1) {
        switch (opt) {
            case 'm':




                        *byteSize = strtoul(( optarg ), NULL, 10);







                break;
            default:
                fprintf(stderr, "Usage: senderf [-m bytesize] \n");
                return -1;
        }
    }


#if DEBUG
    printf("The value of bytesize is %zu\n", *byteSize);
#endif
    return 0;
}