#include <stdio.h>
#include "thread.h"
#include <unistd.h>

void siginthandler(){
    printf("HI I AM SAKSHI!!\n");
  
}

void * print(){
    //printf("CHild\n");
    signal(SIGINT,siginthandler);
    for(int i = 0; i < 20; i++)
        
        printf("Hello world\n");
}


int main(){
    thread_tcb t1;
    int a, b;
    
    
    thread_create(&t1, print, NULL);
    printf("Test file:%d, %d\n", t1.pid, t1.tid);
    thread_kill(t1, SIGINT);
    printf("Parent\n");
    
    sleep(2);
    return 0;
}