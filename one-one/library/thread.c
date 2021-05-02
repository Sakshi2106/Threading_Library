
#include "thread.h"

int thread_count = 1;
long retvalue_array[MAX_THREADS];
static int islock = 0;
Node *head;

//Wrapper function around user start_routine function
int thread_startroutine_execute(void *new_thread){

    lock(&islock);
    //getting return value from function
    ((thread_tcb*)new_thread)->return_value = ((thread_tcb*)new_thread)->function(((thread_tcb*)new_thread)->arg);

    //storing return value in array
    retvalue_array[((thread_tcb*)new_thread)->tid - 1] = (long int)(((thread_tcb*)new_thread)->return_value);
    unlock(&islock);
    return 0;
}

//free the doubly linked list after execution of main function
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

//initialize the threading library
void thread_init(){
    atexit(cleanup);
   
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
   
    //allocate stack to new thread
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
        unlock(&islock);
        munmap(new_thread->stack, STACK_SIZE);
        return errno;
    }

    //add new thread in doubly linked list
    add(&head, *new_thread);
    *thread = *new_thread;
    thread_count++; 
    fflush(stdout);
    unlock(&islock);
    return 0;
}

//send given signal to given thread
int thread_kill(thread_tcb thread, int sig){

    //check whether signal is or not
    if(sig < 0 || sig > 64)
        return EINVAL;

    if(sig > 0)
        kill(thread.pid, sig);
    return 0;
}

//Terminates the calling thread
void thread_exit(void *retval){
    lock(&islock);
    if(retval == NULL)
        return;
    pid_t cur_pid = getpid();
    thread_tcb  *thread = getNodeUsingPid(head, cur_pid);
    unlock(&islock);

    thread -> return_value = retval;
    thread -> status = TERMINATED;
    thread -> ret_threadexit = 1;
   
    kill(cur_pid, SIGKILL);
    
    munmap(thread->stack, STACK_SIZE);
    thread -> stack = NULL;
    return;
}

//joined with a terminated thread
int thread_join(thread_tcb thread, void **retval){
    
    int status;
    lock(&islock);
    
    //if thread state is detached return EINVAL
    if(thread.detach_state == DETACHED){
        unlock(&islock);
        return EINVAL;
    }
    
    //get the target thread
    thread_tcb *joined_thread = getNodeUsingTid(head, thread.tid);
    //if target thread not found in doubly linked list return ESRCH
    if(joined_thread == NULL){
        unlock(&islock);
        return ESRCH;
    }

    //if target thread is already joined thread return EINVAL
    if(joined_thread->waiting_thread_tid != -1){
        unlock(&islock);
        return EINVAL;
    }
   
    if(thread.detach_state == JOINABLE){
    
        unlock(&islock);
        //wait till target thread completes its execution
        waitpid(thread.pid, &status, 0);
        
        lock(&islock);
        
        thread_tcb *req = getNodeUsingPid(head, thread.pid);
        Node *thread_removed = removeNodeWithTid(&head, thread.tid);
        unlock(&islock);
        
        //save the return value in retval
        if(retval != NULL){
            if(thread_removed != NULL){
                
                if(req->ret_threadexit == 0){
                    *retval = (void*)(retvalue_array[joined_thread->tid-1]);
                }
                else if(req->ret_threadexit == 1){
                    *retval = (req->return_value);
                }
            }
            
        }
        lock(&islock);
        thread_count--;
        unlock(&islock);
        return 0;
    }    
    
}

//Change the signal mask of the calling thread
int thread_sigmask(int how, sigset_t *set, sigset_t *oldset) {
        // Set the signal mask 
        sigprocmask(how, set, oldset);
        return 0;
}

