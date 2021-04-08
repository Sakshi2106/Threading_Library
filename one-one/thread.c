
#include "thread.h"

int thread_count = 1;

static int islock = 0;
Node *head;
thread_tcb *p;
//Execute the start routine in clone function
int thread_startroutine_execute(void *new_thread){
   // printf("ENter\n");
    p = (thread_tcb*)new_thread;
    
    (p)->return_value = (p)->function((p)->arg);
    
    //printf("Sum:%d\n", *(int*)((p)->return_value));
   
    //thread_exit(p->return_value);
    //return 0;

}

//Thread create function for creating thread
int thread_create(thread_tcb *thread, void *(*start_routine) (void *), void *arg){

    lock(&islock);
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
    new_thread -> waiting_thread = NULL;
    new_thread -> status = RUNNING;
    new_thread -> ret_threadexit = 0;
    new_thread -> pid = clone(thread_startroutine_execute,  new_thread -> stack + STACK_SIZE, SIGCHLD | CLONE_FS | 
                            CLONE_FILES | CLONE_SIGHAND | CLONE_VM, (void*)new_thread);
    //sleep(1);
    //printf("New thread with pid: %d\n", new_thread->pid);
    new_thread -> tid = thread_count;
    //printf("Adrr:%d\n", *(int*)(new_thread->return_value));
    //printf("Sum:%d\n", *(int*)(((thread_tcb*)new_thread)->return_value));
    if(new_thread -> pid == -1){
        munmap(new_thread->stack, STACK_SIZE);
        return errno;
    }
    add(&head, *new_thread);
    *thread = *new_thread;
    thread_count++; 
    fflush(stdout);
    unlock(&islock);
    return 0;
}

//send given signal to given thread
int thread_kill(thread_tcb thread, int sig){

    //printf("THread pid: %d\n", thread.pid);
    if(sig < 0)
        return EINVAL;
    if(sig > 0)
        kill(thread.pid, sig);
    return 0;
}

void thread_exit(void *retval){
    lock(&islock);
    pid_t cur_pid = getpid();
    unlock(&islock);
    thread_tcb  *thread = getNodeUsingPid(head, cur_pid);
   
    thread -> return_value = retval;
    thread -> status = TERMINATED;
    thread -> ret_threadexit = 1;
   
    kill(cur_pid, SIGKILL);
    
    munmap(thread->stack, STACK_SIZE);
    thread -> stack = NULL;
    
}

int thread_join(thread_tcb thread, void **retval){
    
    int status;
    lock(&islock);
    pid_t curr_pid = getpid();
    unlock(&islock);
    thread_tcb *curr_thread = getNodeUsingPid(head, curr_pid);

    if(thread.detach_state == DETACHED)
        return EINVAL;
    
    
    if(&thread == &(curr_thread -> waiting_thread))
        return EDEADLK;
    
    if(thread.status == JOINED)
        return EINVAL;
    
    if(getNodeUsingTid(head, thread.tid) == NULL)
        return ESRCH;
   
    if(thread.detach_state == JOINABLE){
        thread.status == JOINED;
        thread.waiting_thread = curr_thread;
       
        waitpid(thread.pid, &status, 0);
       
        thread_tcb *req = getNodeUsingTid(head, thread.tid);
        Node *thread_removed = removeNodeWithTid(&head, thread.tid);
        //printf("%d\n", req->ret_threadexit);
        if(retval != NULL){
            if(thread_removed != NULL){
                
                if(req->ret_threadexit == 0){
                    //printf("Sum:%d\n", *(int*)(p->return_value));
                    //  printf("Sum:%d\n", *(int*)(req->return_value));
                    *retval = ((p) -> return_value);
                    //printf("Sum:%d\n", **(int**)(retval));
                }
                else if(req->ret_threadexit == 1){
                
                    *retval = (req->return_value);
                
                }
            }
            
        }
       
        thread_count--;
        free(thread_removed);
        return 0;
    }    
    
}

