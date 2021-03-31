#include <stdio.h>
#include "thread.h"
#include <unistd.h>

void * print(){
    for(int i = 0; i < 20; i++)
        
        printf("Hello world\n");
}

void siginthandler(){
    printf("HI I AM SAKSHI!!\n");
  
}

int main(){
    thread_tcb t1;
    int a, b;
    thread_kill(t1, SIGINT);
    signal(SIGINT,siginthandler);
    thread_create(&t1, print, NULL);
    
    
    sleep(2);
    return 0;
}