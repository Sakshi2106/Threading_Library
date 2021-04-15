#include "thread.h"

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