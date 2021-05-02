#include "thread.h"

int init_queue(queue *p){
   
    p->count = 0;
    p->head = NULL;
    p->tail = NULL;
  
    return 0;
    
}

//enqueue thread in queue
void enqueue(queue *p, thread_tcb *t){
   
    Node *tmp = malloc(sizeof(Node));
    tmp -> thread = t;
    tmp -> next = NULL;
    
    if(p -> tail == NULL){
        
       p -> head = tmp;
       p -> tail = tmp;
    }
    else{
        
        p -> tail -> next  = tmp;
        p -> tail = tmp;
    }
    
    p -> count += 1;
}

//dequeue thread from queue
thread_tcb * dequeue(queue *p){

    if(p -> tail == NULL){
        return NULL;
    }
    else{
        Node *tmp;
        thread_tcb *t1;
        tmp = p -> head;
        t1 = tmp -> thread;
        p -> head = p -> head -> next;
        p -> count--;
        if(p -> head == NULL){
            p -> tail = NULL;
        }
        free(tmp);
        return t1;
    }
}

//checks whether queue is empty
int isempty(queue *p){
    if(p -> head == NULL && p -> tail ==NULL ){
        return 1;
    }
    return 0;
}

//search thread with given tid in queue
thread_tcb *search_with_tid(queue *p, int id){
    
    if(p -> head == NULL)
        return NULL;

    Node *tmp = p -> head;
    while(tmp){
        if(tmp->thread->tid == id)
            return tmp -> thread;
        
        tmp = tmp -> next;
    }
    return NULL;
}

void print_queue(queue *p){
    if(isempty(p))
        return;

    printf("No of nodes in queue: %d\n", p -> count);
    Node *tmp = p -> head;
    while(tmp){
        printf("Tid: %d\n", tmp->thread->tid);
       
        printf("Status: %d\n", tmp->thread->status);
        tmp = tmp -> next;
    }
}

void cleanup_queue(queue *p){

    Node *tmp = p -> head;
    Node *tmp2;
    printf("count: %d\n" , p -> count);
    for(int i = 0; i < p -> count; i++){
        tmp2 = tmp -> next;
        free((tmp -> thread));
        free(tmp);
        tmp = tmp2;
    }    
    p -> count = 0;
    return;
}
