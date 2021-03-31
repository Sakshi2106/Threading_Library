#include <stdio.h>
#include "thread.h"

int print(){
    for(int i = 0; i < 5; i++)
        printf("Hello world\n");
}

int main(){
    thread_tcb t1;
    int a, b;

    thread_create(&t1, print, NULL);
    thread_kill(t1, SIGINT);
    sleep(2);
    return 0;
}