#include "thread.h"

// Initialize spinlock
//Allocate resources for thread
int thread_init(thread_spinlock *lock){

    if(!lock){
        return EINVAL;
    }    

    lock = (thread_spinlock *)malloc(sizeof(thread_spinlock));
    if(!lock){
        return EAGAIN;
    }

    lock -> pid = -1;
    lock -> flag = 0;
    lock -> lock_state = UNLOCKED;

    return 0;
}


//Destroy spinlock
//Free the resources previously initialized for spinlock
int thread_destroy(thread_spinlock *lock){

    if(!lock){
        return EINVAL;
    }

    if(lock -> pid != -1)
        return EINVAL;

    free(lock);
    return 0;
}