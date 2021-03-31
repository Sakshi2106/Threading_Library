#include "thread.h"

/* Node of a doubly linked list */
typedef struct Node {
    thread_tcb tcb;
    struct Node* next; // Pointer to next node in DLL
    struct Node* prev; // Pointer to previous node in DLL
}Node;

void add(Node** head, thread_tcb new_tcb);
thread_tcb* removeNodeWithTid(Node** head, int tid);
thread_tcb* getNodeUsingTid(Node*head, int tid);
int isEmpty(Node *head);