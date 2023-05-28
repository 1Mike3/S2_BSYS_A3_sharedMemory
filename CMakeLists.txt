cmake_minimum_required(VERSION 3.25)
project(sharedMemory C)

set(CMAKE_C_STANDARD 99)

add_executable(sender  s_main.c sender/s_definitions.h)
add_executable(receiver r_main.c receiver/r_definitions.h)

