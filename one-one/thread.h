#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

//Stack size of each thread
#define STACK_SIZE 1024*32

/*Maximum threads per process*/
#define MAX_THREADS 20

/*For thread state*/
#define JOINABLE 1
#define DETACHED 2


typedef struct TCB{

    /*pid of thread*/
    //pid_t pid;

    /*thread id of thread*/
    int tid;

    /*Start position of executing function*/
    //void *(*start_routine) (void *);

    /*Pointer to arguments to the start function*/
    //void *arg;

    /*Detach state of thread*/
   // int detach_state;

    /*Pointer to thread stack*/
    //void *stack;

    /*Pointer to return vaule of start function*/
    //void *return_value;

}thread_tcb;