#include "thread.h"
#include <stdio.h>
#include <unistd.h>
void* fun(void *args){
	while(1){
		printf("Hello World!\n");
		int r = 1000;
		thread_exit(&r);
	}
}	

int fun11(void *args){
	printf("Hi\n");
	int r = 999;
	return 11;
}

int main(){
	
	thread_tcb t1, t2;
	thread_create( &t1, (void*)fun11 , NULL);
	//thread_create( &t2, (void *)fun11 , NULL);	
	void* ret;
    printf("ENter thread join\n");
	thread_join(t1, (&ret));
    printf("return thread join\n");
    printf("%d\n", *(int*)ret);
	printf("\nretval from join of thread1 %d\n", *(int*)(ret));
	// thread_kill(t2, SIGINT);
	// thread_join(t2, &ret);
    // sleep(2);
	// printf("\nretval from join of thread2 %d\n", *(int*)(ret));
    // sleep(2);
	return 0;
}