#include "thread.h"
#include <stdio.h>
#include <unistd.h>
void* fun(int a){
	while(1){
		printf("Hello World!\n");
		int r = 1000;
		int *p = &r;
	    return (void *)p;
	}
}	

void* fun11(void *args){
	printf("Hi\n");
	int r = 999;
    int *p = &r;
	return (void *)p;
}

int main(){
	
	thread_tcb t1, t2;
    thread_init();
    void* ret;

    int a = 4;
	thread_create( &t1, (void*)fun , &a);
    printf("after  1st thread create\n");
    int i = 100;
    // while(i >0){

    //     printf("%d: in while\n", i);
    //     i--;
    // };
    
    printf("after  2st thread create\n");
	thread_create( &t2, (void*)fun11 , NULL);	
    printf("after  2st thread create\n");
	//void* ret;
    //thread_join(t1, (&ret));
    //thread_join(t2, (&ret));
    printf("ENter thread join\n");
 	
     printf("return thread join\n");
//    // printf("%d\n", *(int*)ret);
 	//printf("\nretval from join of thread1 %d\n", *(int*)(ret));
    thread_kill(t2, SIGINT);
//thread_join(t2, &ret);
//     //sleep(2);
	//printf("\nretval from join of thread2 %d\n", *(int*)(ret));
    //sleep(2);
	return 0;
}