#include "thread.h"


// Initialize spinlock
//Allocate resources for thread
int thread_spin_init(thread_spinlock *lock){
    block_interrupt();
    
    if(!lock){
        return EINVAL;
    }    

    lock = (thread_spinlock *)malloc(sizeof(thread_spinlock));
    if(!lock){
        return EAGAIN;
    }

    lock -> pid = -1;
    lock -> lock_state = UNLOCKED;
    lock -> flag = 0;
    
    unblock_interrupt();
    return 0;
}


//Destroy spinlock
//Free the resources previously initialized for spinlock
int thread_spin_destroy(thread_spinlock *lock){
    block_interrupt();
    if(!lock){
        return EINVAL;
    }

    if(lock -> pid != -1)
        return EINVAL;

    free(lock);
    unblock_interrupt();
    return 0;
}

//Acquire lock
int thread_spin_lock(thread_spinlock *lock){
  
    int cur_pid;
   
    if(!lock){
        return EINVAL;
    }

    //while lock status is not acquired
    while(atomic_flag_test_and_set(&(lock->flag)));
 
    cur_pid = getpid();
  
    lock -> pid = cur_pid;
    lock -> lock_state = LOCKED;
   
    return 0;

}

int thread_spin_unlock(thread_spinlock *lock){
    
    int cur_pid;
    if(!lock){
        return EINVAL;
    }

    cur_pid = getpid();

    if(lock -> pid != cur_pid){
        return EINVAL;
    }
    
    lock -> pid = -1;
    lock -> flag = 0;
    lock -> lock_state = UNLOCKED;
    return 0;

}

