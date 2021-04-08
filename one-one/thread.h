#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdatomic.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <sys/time.h>
#include "lock.h"


//Stack size of each thread
#define STACK_SIZE 1024*32

/*Maximum threads per process*/
#define MAX_THREADS 20

/*For thread state*/
#define JOINABLE 1
#define DETACHED 2

/*THread status*/
#define RUNNING 1
#define RUNABLE 2
#define TERMINATED 3
#define WAITING 4
#define JOINED 5

typedef struct TCB{

    /*pid of thread*/
    pid_t pid;

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

}thread_tcb;

/* Node of a doubly linked list */
typedef struct Node {
    thread_tcb tcb;
    struct Node* next; // Pointer to next node in DLL
    struct Node* prev; // Pointer to previous node in DLL
}Node;

void add(Node** head, thread_tcb new_tcb);
Node* removeNodeWithTid(Node** head, int tid);
thread_tcb* getNodeUsingTid(Node*head, int tid);
thread_tcb* getNodeUsingPid(Node*head, pid_t pid);
int isEmpty(Node *head);

int thread_startroutine_execute(void *new_thread);
int thread_create(thread_tcb *thread, void *(*start_routine) (void *), void *arg);
int thread_kill(thread_tcb thread, int sig);
void thread_exit(void *retval);
int thread_join(thread_tcb thread, void **retval);