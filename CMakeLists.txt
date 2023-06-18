cmake_minimum_required(VERSION 3.25)
project(sharedMemory C)

set(CMAKE_C_STANDARD 99)

include_directories(sender/ shared/ receiver/)


add_executable(sender
        s_main.c
        sender/s_parameters.c
        shared/definitions.h
        sender/s_memoryManagement.c
        sender/s_memoryManagement.h shared/sharedFunctions.c shared/sharedFunctions.h)

add_executable(receiver
        r_main.c
        receiver/r_parameters.c
        shared/definitions.h
        receiver/r_memoryManagement.c
        receiver/r_memoryManagement.h shared/sharedFunctions.c shared/sharedFunctions.h)

