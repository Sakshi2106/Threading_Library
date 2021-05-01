#define _GNU_SOURCE
#include "thread.h"
#include <stdio.h>
#include <string.h>

int infinite;
int *p;
void fail(int retval){
    if(retval != 0){
        printf("Error value : %s\n", strerror(retval));
        printf("TEST PASSED\n\n");
    }
    else
        printf("TEST FAILED\n\n");
}

void pass(int retval){
    if(retval == 0)
        printf("TEST PASSED\n\n");
    else
        printf("TEST FAILED\n\n");
}

void check(int retval){
    if(retval != 0){
        printf("Error value : %s\n\n", strerror(retval));
    }
}


void handler(){
    printf("Inside handler\n");
	infinite = 0;
}

void *thread1(){
    printf("In THREAD1\n");
    int r = 1;
    p = &r;
    return p;
}

void *thread2(){
    int r = 100;
    thread_exit(&r);
}

void *thread3(){
    printf("In THREAD2\n");
    int r = 1;
    p = &r;
    return p;
}

void *thread4(){
    printf("In thread having infinite loop\n");
    int r = 29;
    thread_tcb t3;
    while(infinite);
    
    int *p = &r;
    return p;
}

void* fun(void *args){
    int r = 1000;
    r = r *2;
    p = &r;
    return p;     
}	
void* fun11(void *args){
       
        int r = 999;
        thread_tcb t3;
        void * ret;
        thread_create(&t3, fun, NULL);
        long int i = 1000000;
        while(i){
            i--;
        }
        thread_join(t3, &ret);
        p = &r;
        return p;
    }

int main(){

    thread_init();
    printf("1] Thread Create testing\n");
    printf("....................................\n");

    printf("Test 1: Start Routine zero\n");{
        thread_tcb t1;
        fail(thread_create( &t1, NULL , NULL));
    }

    printf("Test 2: Create thread with default attributes\n");{
        thread_tcb tid;
        check(thread_create(&tid, thread1, NULL));
        check(thread_join(tid, NULL));
        printf("TEST PASSED\n\n");
    }
    //raise(SIGTSTP);
    printf("....................................\n");
    printf("2] Thread Join\n");
    printf("....................................\n");
    printf("Test 3: Invalid thread_tcb passed\n");
    {
        thread_tcb tid1;
        check(thread_create(&tid1, thread1, NULL));
        thread_tcb t2;
        t2.tid = 888;
        fail(thread_join(t2, NULL));
    }

    printf("Test 4: Joining on already joined thread\n");{
        thread_tcb tid2;
        check(thread_create(&tid2, thread1, NULL));
        check(thread_join(tid2, NULL));
        fail(thread_join(tid2, NULL));
    }
    printf("Test 5: Joining on thread and getting the thread value\n");
    {
        thread_tcb tid3;
        check(thread_create(&tid3, thread2, NULL));
        void *ret;
        check(thread_join(tid3, &ret));
        printf("Expected return value: 100\n");
        printf("Actual return value: %d\n", *(int*)ret);
        if(*(int*)ret == 100)
            printf("TEST PASSED\n\n");
        else
            printf("TEST FAILED\n\n");
    }
    printf("Test 6: Joining on more than one thread\n");
    {
        thread_tcb tid[5];
        for(int i = 0; i < 5; i++) {
            check(thread_create(&tid[i], thread3, NULL));
            printf("Thread %d created\n", i);
        }
        for(int i = 0; i < 5; i++) {
            check(thread_join(tid[i], NULL));
            printf("Thread %d joined\n", i);
        }
        printf( "TEST PASSED\n\n");
    }

    printf("-------------------------------------------\n");
    printf("3] Thread Kill\n");
    printf("-------------------------------------------\n");
    printf("Test 7: Send invalid signal\n");
    {
        thread_tcb tid;
        signal(SIGUSR1, handler);
        infinite = 1;
        check(thread_create(&tid, thread4, NULL));
        check(thread_kill(tid, -1));
        infinite = 0;
        check(thread_join(tid, NULL));
        printf("TEST PASSED\n\n");
    }

    printf("Test 8: Send signal to a thread\n");
    {
        void *value;
        thread_tcb tid;
        signal(SIGUSR1, handler);
        infinite = 1;
        check(thread_create(&tid, thread4, NULL));
        check(thread_kill(tid, SIGUSR1));
        check(thread_join(tid, &value));
        if(*(int*)value == 29) {
            printf("TEST PASSED\n\n");
        }
        else {
            printf("TEST FAILED\n\n");
           
        }
    }
    printf("-------------------------------------------\n");
    printf("4] Thread Exit\n");
    printf("-------------------------------------------\n");
    printf("Test 9: Return value using thread_exit\n");
    {
        void *value;
        thread_tcb tid;

        check(thread_create(&tid, thread2, NULL));
        check(thread_join(tid, &value));
        printf("Expected return value: %d\n", 100);
        printf("Actual return value: %d\n", *(int*) value);
        if(*(int*)value == 100) {
            printf("TEST PASSED\n\n");
        }
        else {
            printf("TEST FAILED\n\n");
        }
    }

    printf("Test 10: Return value using return\n");
    {
        void *value;
        thread_tcb tid;

        check(thread_create(&tid, thread4, NULL));
        check(thread_join(tid, &value));
        printf("Expected return value: %d\n", 29);
        printf("Actual return value: %d\n", *(int*) value);
        if(*(int*)value == 29) {
            printf("TEST PASSED\n\n");
        }
        else {
            printf("TEST FAILED\n\n");
        }
    }

    printf("-------------------------------------------\n");
    printf("Test 11: Creating a thread inside thread\n");
    {
        void *value;
        thread_tcb tid;

        check(thread_create(&tid, fun11, NULL));
        check(thread_join(tid, &value));
        printf("Expected return value: %d\n", 999);
        printf("Actual return value: %d\n", *(int*) value);
        if(*(int*)value == 999) {
            printf("TEST PASSED\n\n");
        }
        else {
            printf("TEST FAILED\n\n");
        }
    }
    return 0;

}
