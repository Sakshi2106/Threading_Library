
#include "thread.h"

int thread_count = 1;

Node *head;

//Execute the start routine in clone function
int thread_startroutine_execute(void *new_thread){

    thread_tcb *p = (thread_tcb*)new_thread;
    p -> return_value = p -> function(p -> arg);
    return 0;

}

//Thread create function for creating thread
int thread_create(thread_tcb *thread, void *(*start_routine) (void *), void *arg){

    thread_tcb *new_thread = (thread_tcb*)malloc(sizeof(thread_tcb));
    if(new_thread == NULL){
		printf("Memory allocation error\n");
		exit(0);
	}
   
	new_thread -> stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
	if(new_thread -> stack == MAP_FAILED){
		printf("Memory allocation error\n");
		exit(0);
	}
    new_thread -> function = start_routine;
    new_thread -> arg = arg;
    new_thread -> detach_state = JOINABLE;
    new_thread -> return_value = NULL;
    
    new_thread -> status = RUNNING;
    new_thread -> pid = clone(thread_startroutine_execute,  new_thread -> stack + STACK_SIZE, SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, (void*)new_thread);
    new_thread -> tid = thread_count;

    if(new_thread -> pid == -1){
        munmap(new_thread->stack, STACK_SIZE);
        return errno;
    }
    add(&head, *new_thread);
    thread_count++; 
    return 0;
}

//send given signal to given thread
int thread_kill(thread_tcb thread, int sig){

    if(sig < 0)
        return EINVAL;
    if(sig > 0)
        kill(thread.pid, sig);
    return 0;
}