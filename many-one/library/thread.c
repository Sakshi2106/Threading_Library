
#include "thread.h"

thread_tcb *current_thread;

int thread_count = 0;
int a;
jmp_buf env;
queue *allthreads;
long retvalue_array[MAX_THREADS];

//blocks the SIGALRM signal
void block_interrupt() {
    sigset_t block;
    sigemptyset(&block);
    sigaddset(&block, SIGALRM);
    sigprocmask(SIG_BLOCK, &block, NULL);
    
}

//unblocks the SIGALRM signal
void unblock_interrupt(){
    sigset_t block;
    sigemptyset(&block);
    sigaddset(&block, SIGALRM);
    sigprocmask(SIG_UNBLOCK, &block, NULL);
    
}

//unmapp the mapped regions and free the queue
void cleanup(void){
   
    struct itimerval timer = {
			{ 0, 0 },
			{ 0, 0 }
		};
		
	setitimer(ITIMER_REAL, &timer, 0);
	free(current_thread);
    
    thread_tcb *p;
    int n = allthreads -> count;
    for(int i = 0; i < n; i++){
        p = dequeue(allthreads);
        munmap(p -> stack, STACK_SIZE);
        p -> stack = NULL;
        free(p);
    }
    thread_count = 0;
    free(allthreads);
		
}

//initialize the threading library and store the context of main thread
void thread_init() {

    allthreads = (queue *)malloc(sizeof(queue));
    if(allthreads == NULL)
        return;
    
    atexit(cleanup);

    //initialize the queue
    init_queue(allthreads);

    thread_tcb *main_thread = (thread_tcb *)malloc(sizeof(thread_tcb));

    if(main_thread == NULL)
        return;

    main_thread -> tid = thread_count;
    main_thread -> function = NULL;
    main_thread -> arg = NULL;
    main_thread -> detach_state = JOINABLE;
    main_thread -> status = RUNNING;
    main_thread -> stack = NULL;
    main_thread -> return_value = NULL;
    main_thread -> waiting_thread_tid = -1;

    sigemptyset(&(main_thread -> pending_signals));

    current_thread = main_thread;
    thread_count ++;
    init_timer();

}

//initialize the timer
void init_timer(){

    sigset_t blocked_mask;
    sigfillset(&blocked_mask);
    
    struct sigaction sigalarm_handler;
    memset(&sigalarm_handler, 0, sizeof(sigalarm_handler));
    sigalarm_handler.sa_handler = scheduler;
	sigalarm_handler.sa_mask = blocked_mask;
	sigalarm_handler.sa_flags = SA_RESTART;
   
    //set Scheduler as handler
    sigaction(SIGALRM, &sigalarm_handler, NULL);
  
    struct itimerval timer;
    timer.it_interval.tv_sec =0 ;
    timer.it_interval.tv_usec =100;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 100;
	//Enabling timer
	setitimer(ITIMER_REAL, &timer, 0);
   
}

//gives the next ready thread in queue
thread_tcb *next_ready_thread(queue *q){
    
    thread_tcb *tmp ;
    for(int i = 0; i < (q->count); i++){
        tmp = dequeue(q);
       
        if(tmp -> status == READY){
            
            return tmp;
        }
            
        enqueue(q, tmp);
    }
    return NULL;
}

//Schedules the thread after the timer interrupt
void scheduler(){
   
    //store the context of running thread
    if(setjmp(current_thread -> context)){
        
        sigset_t all;
        sigfillset(&all);
        sigdelset(&all, SIGALRM);
        sigprocmask(SIG_UNBLOCK, &all, NULL);

        for(int i = 0; i < NSIG; i++){
            if(sigismember(&(current_thread->pending_signals), i)) {
                raise(i);
                sigdelset(&(current_thread->pending_signals), i);
            }
        }

        return ;
    }
        
    //if state of thread is not terminated change the state to READY
    if(current_thread -> status != TERMINATED){
        current_thread -> status = READY;
    }
    
    enqueue(allthreads, current_thread);
    //get next ready thread from queue
    current_thread = next_ready_thread(allthreads);
    
    if(current_thread == NULL){
        exit(0);
    }
    //change the state to RUNNING and restore the context
    current_thread -> status = RUNNING;
    longjmp(current_thread -> context, 1);

}

//Wrapper function around user start_routine function
void thread_startroutine_execute(void ){
    unblock_interrupt();
    current_thread->return_value = current_thread->function(current_thread->arg);
    thread_exit(current_thread->return_value);
    return;
}

//Creates the new thread
int thread_create(thread_tcb *thread, void *(*start_routine) (void *), void *arg){
   
    block_interrupt();
    if(thread == NULL)
        return EAGAIN;
    
    if(thread_count > MAX_THREADS)
        return EAGAIN;
    if(start_routine == NULL)
        return EAGAIN;
    thread_tcb *new_thread = (thread_tcb*)malloc(sizeof(thread_tcb));
    if(new_thread == NULL){
		printf("Memory allocation error\n");
		exit(0);
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
    new_thread -> status = READY;
    new_thread -> tid = thread_count;
    sigemptyset(&(new_thread -> pending_signals));

    setjmp(new_thread->context);
    
    new_thread->context[0].__jmpbuf[JB_RSP] = i64_ptr_mangle((long int) new_thread->stack + STACK_SIZE - sizeof(long int));
   
	new_thread->context[0].__jmpbuf[JB_PC] = i64_ptr_mangle((long int) thread_startroutine_execute);
    enqueue(allthreads, new_thread);
    *thread = *new_thread;
   
    thread_count++;
    unblock_interrupt();
   
    return 0;
}

//Terminates the calling thread
void thread_exit(void *retval){
   
    block_interrupt();
    current_thread->status  = TERMINATED;
    current_thread->return_value = retval;

    //If any thread is waiting for the terminated thread change the state of that thread to READY and raise SIGALRM
    if(current_thread->waiting_thread_tid != -1) {
       
        thread_tcb *waiting_thread = search_with_tid(allthreads, current_thread->waiting_thread_tid);
        waiting_thread->status   = READY;
    }
    
    unblock_interrupt();
    raise(SIGALRM);
    
}

//Join with terminated thread
int thread_join(thread_tcb thread, void **retval){
    block_interrupt();
   
    //if thread state is detached return EINVAL
    if(thread.detach_state == DETACHED){
        unblock_interrupt();
        return EINVAL;
    }
    
    //If there id Deadlock return EDEADLK
    if(thread.tid == (current_thread -> waiting_thread_tid)){
        unblock_interrupt();
        return EDEADLK;
    }

    //search the target thread
    thread_tcb *joined_thread = search_with_tid(allthreads, thread.tid);

    //if target thread not found return ESRCH
    if(joined_thread == NULL)
        return ESRCH;

    //if target thread is already joined return EINVAL
    if(joined_thread->waiting_thread_tid != -1){
        unblock_interrupt();
        return EINVAL;
    }
   

    if(thread.detach_state == JOINABLE){
       
        joined_thread->waiting_thread_tid = current_thread -> tid;
        current_thread -> status = WAITING;
        unblock_interrupt();
        //wait till target thread status changes to TERMINATED
        while(joined_thread->status != TERMINATED);
        block_interrupt();
       
        //store the return value
        if(retval){
            *retval = joined_thread->return_value;
        }
       
    }  
    unblock_interrupt();  
    return 0;
}

//send signal to particular thread
int thread_kill(thread_tcb thread, int sig){
    block_interrupt();
    if(sig < 0 || sig > 64)
        return EINVAL;
    
    if(thread.tid == current_thread->tid )
        raise(sig);
    
   
    block_interrupt();
    thread_tcb *target_thread = search_with_tid(allthreads ,thread.tid);
    if(target_thread == NULL) {
        unblock_interrupt();
        return EINVAL;
    }
    //store pending signals
    sigaddset(&target_thread->pending_signals, sig);
    unblock_interrupt();
    return 0;
}

//Yields the processor
void thread_yeild(void){
    raise(SIGALRM);
}

//Change the signal mask of calling thread
int thread_sigmask(int how, sigset_t *set, sigset_t *oldset) {
    // Set the signal mask 
    sigprocmask(how, set, oldset);
    return 0;
}