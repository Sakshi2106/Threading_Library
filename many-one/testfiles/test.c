#include <stdio.h>

#include "thread.h"

int print(){
    for(int i = 0; i < 5; i++)
        printf("Hello world\n");
    return 0;
}
int print2(){
    for(int i = 0; i < 5; i++)
        printf("HI\n");
    return 0;
}

int main(){
    thread_tcb t1, t2;
    int a, b;
    thread_init();
    thread_create(&t1, (void*)print, NULL);
    thread_create(&t2, (void*)print2, NULL);
    sleep(2);
    return 0;
}