#include <string.h>
#include "thread.h"
 
thread_mutex lock1;
long c = 0, c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0, c6 = 0, run = 1;

void check(int retval){
    if(retval != 0){
        printf("Error value : %s\n\n", strerror(retval));
    }
}

void *thread1(void *arg) {
	while(run == 1) {
		thread_mutex_lock(&lock1);
		c++;
		thread_mutex_unlock(&lock1);
		c1++;
	}
}
void *thread2(void *arg) {
	while(run == 1) {
		thread_mutex_lock(&lock1);
		c++;
		thread_mutex_unlock(&lock1);
		c2++;
	}
}
void *thread3(void *arg) {
	while(run == 1) {
		thread_mutex_lock(&lock1);
		c++;
		thread_mutex_unlock(&lock1);
		c3++;
	}
}
void *thread4(void *arg) {
	while(run == 1) {
		thread_mutex_lock(&lock1);
		c++;
		thread_mutex_unlock(&lock1);
		c4++;
	}
}
void *thread5(void *arg) {
	while(run == 1) {
		thread_mutex_lock(&lock1);
		c++;
		thread_mutex_unlock(&lock1);
		c5++;
	}
}
void *thread6(void *arg) {
	while(run == 1) {
		thread_mutex_lock(&lock1);
		c++;
		thread_mutex_unlock(&lock1);
		c6++;
	}
}
int main() {
	thread_tcb th1, th2, th3, th4, th5, th6;
    printf("Mutex Testing\n");
    thread_init();
	check(thread_mutex_init(&lock1)); 
	check(thread_create(&th1, thread1, NULL));
	check(thread_create(&th2, thread2, NULL));
    check(thread_create(&th3, thread3, NULL));
    check(thread_create(&th4, thread4, NULL));
    check(thread_create(&th5, thread5, NULL));
    check(thread_create(&th6, thread6, NULL));
	
    long int i = 1000000000;
    while(i){
        i--;
    }

	run = 0;
	check(thread_join(th1, NULL));
	check(thread_join(th2, NULL));
    check(thread_join(th3, NULL));
    check(thread_join(th4, NULL));
    check(thread_join(th5, NULL));
    check(thread_join(th6, NULL));

	printf("Thread 1 : c1 = %ld\n", c1);
	printf("Thread 2 : c2 = %ld\n", c2);
	printf("Thread 3 : c3 = %ld\n", c3);
	printf("Thread 4 : c4 = %ld\n", c4);
	printf("Thread 5 : c5 = %ld\n", c5);
    printf("Thread 6 : c6 = %ld\n", c6);
	printf("c1 + c2 + c3 + c4 + c5 + c6 = %ld\n", c1+c2+c3+c4+c5+c6);
	printf("Shared Variable  : c = %ld\n", c);
	
	if(c == c1+c2+c3+c4+c5+c6)
        printf("TEST PASSED\n");
    else
        printf("TEST FAILED\n");

    return 0;
}