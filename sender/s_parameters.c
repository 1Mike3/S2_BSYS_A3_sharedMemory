//
// Created by michi on 5/28/23.
//

#include "s_parameters.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 1

short manageParameters(int argc, char *argv[], size_t *byteSize, char ** data) {

    char * dataInternal = calloc(sizeof(char), *byteSize);

    //get opt inmplementation to let the user enter bytesize with the -m flag
    int opt;
    short parCounter = 0;
    int dataSize = 0;
    while ((opt = getopt(argc, argv, "m:")) != -1) {
        switch (opt) {
            case 'm':


                 //loop to get data from two argument pard
                optind--;
                for( ;optind < argc && *argv[optind] != '-'; optind++){

                    //ERROR if statement for NULL args
                    if (( argv[optind] ) == NULL || ( argv[optind] ) == 0) {
                        fprintf(stderr, "Usage: sender [-m bytesize] \n");
                        return -1;
                    }

                    if(parCounter == 0) {
                        *byteSize = strtoul(( argv[optind] ), NULL, 10);
                        parCounter++;
                    }else if(parCounter == 1 ){

                        //ERROR section for too much data
                        //get the size of the data and compare it to the size of the buffer if too much error before writing
                        char temp = ( argv[optind][0] );
                        int i = 0;
                        while (temp != '\0' && temp != EOF) {
                            temp = ( argv[optind][i] );
                            dataSize++;
                            i++;
                        }

                        if(dataSize >= *byteSize){
                            fprintf(stderr, "[S] Too much data for the specified Buffer \n");
                            return -1;
                        }

                        strncpy(dataInternal,( argv[optind] ), dataSize );
                        *data = dataInternal;

                        parCounter++;
                    }
                }




                break;
            default:
                fprintf(stderr, "Usage: sender [-m bytesize] \n");
                return -1;
        }
    }

    //ERROR if no Data passed
    if(parCounter < 2){
        fprintf(stderr, "Usage: sender [-m bytesize] \n");
        return -1;
    }

#if DEBUG
    printf("The value of bytesize is %zu\n", *byteSize);
#endif
    return 0;
}