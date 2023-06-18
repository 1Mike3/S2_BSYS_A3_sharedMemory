cmake_minimum_required(VERSION 3.25)
project(sharedMemory C)

set(CMAKE_C_STANDARD 99)

include_directories(senderf/ shared/ receiverf/)

find_package(Threads REQUIRED)

add_executable(sender
        s_main.c
        senderf/s_parameters.c
        shared/definitions.h
        senderf/s_memoryManagement.c
        senderf/s_memoryManagement.h shared/sharedFunctions.c shared/sharedFunctions.h)

target_link_libraries(sender rt Threads::Threads)

add_executable(receiver
        r_main.c
        receiverf/r_parameters.c
        shared/definitions.h
        receiverf/r_memoryManagement.c
        receiverf/r_memoryManagement.h shared/sharedFunctions.c shared/sharedFunctions.h)

target_link_libraries(receiver rt)

target_link_libraries(receiver rt Threads::Threads)