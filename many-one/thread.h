#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>


//Stack size of each thread
#define STACK_SIZE 1024*64

/*Maximum threads per process*/
#define MAX_THREADS 50

/*For thread state*/
#define JOINABLE 1
#define DETACHED 2

/*THread status*/
#define RUNNING 1
#define READY 2
#define TERMINATED 3
#define WAITING 4
#define JOINED 5

#define JB_RSP 6
#define JB_PC 7

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
    int waiting_thread_tid;


    /*For pending signals of the thread*/
    sigset_t pending_signals;

    /*Context of thread*/
    jmp_buf context;
}thread_tcb;

typedef struct Node {

    thread_tcb *thread;
    struct Node *next;
}Node;

typedef struct queue {

    Node *head;
    Node *tail;
    int count;
}queue;

int init_queue(queue *p);

void enqueue(queue *p, thread_tcb *t);

thread_tcb *dequeue(queue *p);

int isempty(queue *p);

thread_tcb *search_with_tid(queue *p, int id);

void print_queue(queue *p);

void cleanup_queue(queue *p);

void block_interrupt();
void unblock_interrupt();

void thread_init();
void init_timer();
void scheduler();
long int i64_ptr_mangle(long int p);
thread_tcb *next_ready_thread(queue *q);

void thread_startroutine_execute(void);
int thread_create(thread_tcb *thread, void *(*start_routine) (void *), void *arg);
int thread_kill(thread_tcb thread, int sig);
void thread_exit(void *retval);
int thread_join(thread_tcb thread, void **retval);
void thread_yeild(void);