# Assignment 3 Kuc

## Compile Information
* The executable has to be in a separate folder from the source files in the same <br>
  parent directory for the paths to work .<br> <br>
* Programm was developed in Clion

## General
* I am using r_ and s_ to differentiate between the files associated with the <br>
receiver and sender executable
* Note to self: for your own sanity only work on s or r at a time
* I created a shared folder which contains resources that are used by both S&R
* allocating memory for both the ringbuffer structure and the ringbuffer itself (SM)
* The programms generate a file each on startup containing the pid for easierer process management
The PID is also saved in the ringbuffer structure for COM between Processes
* I am Using Semaphores for the Read and Write operations to ensure that the ringbuffer is not accessed by multiple processes at the same time
* If you don't want to see the Signal status messages Set DEBUG_SIG to 0 in the definitions.h header file

## Programming notes
* create ringbuffer should be changed to initalize ringbuffer as it is kinda useless now after the changes

## Known Bugs
* The sender has to be started befre the receiver, i tried to implement this the other way around too but it didn't work
* the sending, receiving and diff part worked just as in the assignment description 