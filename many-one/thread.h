#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <assert.h>

typedef struct TCB{

    
    /*thread id of thread*/
    int tid;

    /*Start position of executing function*/
    void *(*function) (void *);

    /*Pointer to arguments to the start function*/
    void *arg;

    /*Detach state of thread*/
    int detach_state;

    /*Status of thread*/
    int status;

    /*Pointer to thread stack*/
    void *stack;

    /*Pointer to return vaule of start function*/
    void *return_value;

    /*Waiting thread*/
    struct TCB *waiting_thread;

    int ret_threadexit;

    /*For pending signals of the thread*/
    sigset_t pending_signals;

    /*Context of thread*/
    jmp_buf context;
}thread_tcb;