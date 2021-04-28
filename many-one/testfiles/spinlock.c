
#include "thread.h"
 
thread_spinlock lock;
long c = 0, c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0, c6 = 0, run = 1;

void check(int retval){
    if(retval != 0){
        printf("Error value : %s\n\n", strerror(retval));
    }
}

void *thread1(void *arg) {
	while(run == 1) {
		thread_spin_lock(&lock);
		c++;
		thread_spin_unlock(&lock);
		c1++;
	}
}
void *thread2(void *arg) {
	while(run == 1) {
		thread_spin_lock(&lock);
		c++;
		thread_spin_unlock(&lock);
		c2++;
	}
}
void *thread3(void *arg) {
	while(run == 1) {
		thread_spin_lock(&lock);
		c++;
		thread_spin_unlock(&lock);
		c3++;
	}
}
void *thread4(void *arg) {
	while(run == 1) {
		thread_spin_lock(&lock);
		c++;
		thread_spin_unlock(&lock);
		c4++;
	}
}
void *thread5(void *arg) {
	while(run == 1) {
		thread_spin_lock(&lock);
		c++;
		thread_spin_unlock(&lock);
		c5++;
	}
}
void *thread6(void *arg) {
	while(run == 1) {
		thread_spin_lock(&lock);
		c++;
		thread_spin_unlock(&lock);
		c6++;
	}
}
int main() {
	thread_tcb th1, th2, th3, th4, th5, th6;
    printf("Spinlock Testing\n");
    thread_init();
	check(thread_spin_init(&lock)); 
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