
#include "thread.h"

int thread_count = 1;
long retvalue_array[MAX_THREADS];
static int islock = 0;
Node *head;

//Execute the start routine in clone function
int thread_startroutine_execute(void *new_thread){

    lock(&islock);
    ((thread_tcb*)new_thread)->return_value = ((thread_tcb*)new_thread)->function(((thread_tcb*)new_thread)->arg);
   
    retvalue_array[((thread_tcb*)new_thread)->tid - 1] = (long int)(((thread_tcb*)new_thread)->return_value);
    //thread_exit( ((thread_tcb*)new_thread)->return_value);
    unlock(&islock);
    
}
void cleanup(){
    Node *tmp;
    tmp = head;
    for(int i = 0; i < MAX_THREADS; i++)
        retvalue_array[i] = 0;

    while(tmp->next != NULL){
        Node *tmp1 = tmp;
        tmp = tmp -> next;
        free(tmp1);
    }
}
void thread_init(){
    atexit(cleanup);
    // for(int i = 0; i < MAX_THREADS; i++)
    //     retvalue_array[i] = 0;

}
//Thread create function for creating thread
int thread_create(thread_tcb *thread, void *(*start_routine) (void *), void *arg){

    lock(&islock);
    thread_tcb *new_thread = (thread_tcb*)malloc(sizeof(thread_tcb));
   
    if(new_thread == NULL){
		printf("Memory allocation error\n");
		exit(0);
	}
    if(start_routine == NULL){
        return EINVAL;
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
    new_thread -> waiting_thread_tid = -1;
    new_thread -> status = RUNNING;
    new_thread -> ret_threadexit = 0;
    new_thread -> tid = thread_count;
    new_thread -> pid = clone(thread_startroutine_execute,  new_thread -> stack + STACK_SIZE, SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, new_thread);
   
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
    if(sig < 0 || sig > 64)
        return EINVAL;
    if(sig > 0)
        kill(thread.pid, sig);
    return 0;
}

void thread_exit(void *retval){
    lock(&islock);
    pid_t cur_pid = getpid();
    thread_tcb  *thread = getNodeUsingPid(head, cur_pid);
    unlock(&islock);

    thread -> return_value = retval;
    thread -> status = TERMINATED;
    thread -> ret_threadexit = 1;
   
    kill(cur_pid, SIGKILL);
    
    // munmap(thread->stack, STACK_SIZE);
    // thread -> stack = NULL;
    
}

int thread_join(thread_tcb thread, void **retval){
    
    int status;
    lock(&islock);
    pid_t curr_pid = getpid();
    
    //thread_tcb *curr_thread = getNodeUsingPid(head, curr_pid);
    // printList(head);
    
    if(thread.detach_state == DETACHED){
        unlock(&islock);
        return EINVAL;
    }
    
    // if(thread.tid == (curr_thread -> waiting_thread_tid)){
    //     unlock(&islock);
    //     return EDEADLK;
    // }
  
    thread_tcb *joined_thread = getNodeUsingTid(head, thread.tid);
    if(joined_thread == NULL){
        unlock(&islock);
        return ESRCH;
    }
    if(joined_thread->waiting_thread_tid != -1){
        unlock(&islock);
        return EINVAL;
    }
   
    if(thread.detach_state == JOINABLE){
       //  printf("..............\n");
        joined_thread->status == JOINED;
        //printf("current thread tid: %d\n", curr_thread->tid);
        //joined_thread->waiting_thread_tid = curr_thread->tid;
        //printf("..............\n");
        unlock(&islock);
        int pid = waitpid(thread.pid, &status, 0);
        
        lock(&islock);
        //printf("..............\n");
        thread_tcb *req = getNodeUsingPid(head, thread.pid);
        Node *thread_removed = removeNodeWithTid(&head, thread.tid);
        unlock(&islock);
        
        if(retval != NULL){
            if(thread_removed != NULL){
                
                if(req->ret_threadexit == 0){
                  
                    *retval = (void*)(retvalue_array[joined_thread->tid-1]);
                    //printf("Sum:%d\n", **(int**)(retval));
                }
                else if(req->ret_threadexit == 1){
                   
                    *retval = (req->return_value);
                    //*retval = (void*)(retvalue_array[thread.tid-1]);
                }
            }
            
        }
        lock(&islock);
        thread_count--;
        unlock(&islock);
        //free(thread_removed);
        return 0;
    }    
    
}

