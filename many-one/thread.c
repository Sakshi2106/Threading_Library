#include "thread.h"
#include "queue.h"

thread_tcb *current_thread;

int thread_count = 0;

queue *allthreads;
struct itimerval timer;

void block_interrupt() {
    sigset_t block;
    sigemptyset(&block);
    sigaddset(&block, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &block, NULL);
}

void unblock_interrupt(){
    sigset_t block;
    sigemptyset(&block);
    sigaddset(&block, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &block, NULL);
}

void thread_init() {

    allthreads = (queue *)malloc(sizeof(queue));
    if(allthreads == NULL)
        return;
   
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

    sigemptyset(main_thread -> pending_signals);

    current_thread = main_thread;
    thread_count ++;    

}

void init_timer(){

    sigset_t blocked_mask;
    sigfillset(&blocked_mask);
    

    const struct sigaction sigalarm_handler = {
        .sa_handler = &scheduler,
		.sa_mask = blocked_mask,
		.sa_flags = SA_RESTART
    }

    struct sigaction *old;

    sigaction(SIGVTALRM, &sigalarm_handler, old);
    
    timer = {
		{ 0, 10 },	
		{ 0, 1 }	
	};
	/*Enabling timer*/
	if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == - 1) {
		sigaction(SIGVTALRM, old, NULL);
	}
	
}

thread_tcb *next_ready_thread(queue *q){
    thread_tcb *tmp ;
    for(int i = 0; i < (q->count); i++){
        tmp = dequeue(q);
        if(tmp -> status == READY)
            return tmp;
        enqueue(q, tmp);
    }
    return NULL;
}

void scheduler(){

    if(setjmp(current_thread -> context)){
        for(int i = 0; i < NSIG; i++){
            if(sigismember(&(current->pending_signals), i)) {
                raise(i);
                sigdelset(&(current->pending_signals), i);
            }
        }
        return;
    }
        

    if(current_thread -> state == RUNNING)
        current_thread -> state = READY;

    enqueue(allthreads, current_thread); 

    current_thread = next_ready_thread(allthreads);
    if(current_thread == NULL)
        return;

    current_thread -> state = RUNNING;

    sigset_t all;
    sigfillset(&all);
    sigprocmask(SIG_UNBLOCK, &all, NULL);

    longjmp(current_thread -> current, 1);

}

void thread_startroutine_execute(void *new_thread){
    ((thread_tcb*)new_thread)->return_value = ((thread_tcb*)new_thread)->function(((thread_tcb*)new_thread)->arg);
}

int thread_create(thread_tcb *thread, void *(*start_routine) (void *), void *arg){

    if(thread == NULL)
        return EAGAIN;

    if(thread_count > MAX_THREADS)
        return EAGAIN;

    if(thread_count == 0){
        thread_init();
        init_timer();
    }

    thread_tcb *new_thread = (thread_tcb*)malloc(sizeof(thread_tcb));
   
    if(new_thread == NULL){
		printf("Memory allocation error\n");
		exit(0);
	}
   
    block_interrupt();
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
    new_thread -> tid = thread_count;
    sigemptyset(new_thread -> pending_signals);

    setjmp(new_thread->context);

    new_thread->context[0].__jmpbuf[JB_RSP] = i64_ptr_mangle((long int) new_thread->stack + STACK_SIZE - sizeof(long int));
   
	new_thread->context[0].__jmpbuf[JB_PC] = i64_ptr_mangle((long int) thread_startroutine_execute);

    enqueue(allthreads, new_thread);
    *thread = *new_thread;

    unblock_interrupt();
    return 0;
}