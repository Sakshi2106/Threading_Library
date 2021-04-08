#include "thread.h"


// Initialize spinlock
//Allocate resources for thread
int thread_spin_init(thread_spinlock *lock){

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
    return 0;
}


//Destroy spinlock
//Free the resources previously initialized for spinlock
int thread_spin_destroy(thread_spinlock *lock){

    if(!lock){
        return EINVAL;
    }

    if(lock -> pid != -1)
        return EINVAL;

    free(lock);
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
   // printf("out\n");
    return 0;

}

int thread_spin_unlock(thread_spinlock *lock){

    int cur_pid;
    //int locked = LOCKED;
    //printf("in unlock]\n");
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


//Mutex locks

static int futex(int *uaddr, int futex_op, int val, const struct timespec *timeout, int *uaddr2, int val3){

    return syscall(SYS_futex, uaddr, futex_op, val,timeout, uaddr, val3);
}


static void thread_wait(int *flag){
    int s;

    while (1) {

        // Is the lock acquired
        const int locked = 0;
        if (atomic_compare_exchange_strong(flag, &locked, 1))
            break;      

        // Lock is not available; wait 

        s = futex(flag, FUTEX_WAIT, 0, NULL, NULL, 0);
        if (s == -1 && errno != EAGAIN)
            printf("futex-FUTEX_WAIT");
    }
}


static void thread_wake(int *flag){

    int s;

    const int unlocked = 1;
    if (atomic_compare_exchange_strong(flag, &unlocked, 0)) {
        s = futex(flag, FUTEX_WAKE, 1, NULL, NULL, 0);
        if (s  == -1)
            printf("futex-FUTEX_WAKE");
    }
}

//Initialize the mutex lock
//Allocate resources for thread
int thread_mutex_init(thread_mutex *lock){

    if(!lock){
        return EINVAL;
    }    

    lock = (thread_mutex *)malloc(sizeof(thread_mutex));
    if(!lock){
        return EAGAIN;
    }

    lock -> pid = -1;
    lock -> lock_state = UNLOCKED;
    lock -> flag = 0;
    return 0;
}

//Destroy spinlock
//Free the resources previously initialized for spinlock
int thread_mutex_destroy(thread_mutex *lock){

    if(!lock){
        return EINVAL;
    }

    if(lock -> pid != -1)
        return EINVAL;

    free(lock);
    return 0;
}

//Acquires lock on mutex object
int thread_mutex_lock(thread_mutex *lock){

    int cur_pid;
   
    if(!lock){
        return EINVAL;
    }

    thread_wait(&(lock->flag));

    cur_pid = getpid();
  
    lock -> pid = cur_pid;
    lock -> lock_state = LOCKED;
   // printf("out\n");
    return 0;
}

int thread_mutex_unlock(thread_mutex *lock){

    int cur_pid;
    
    if(!lock){
        return EINVAL;
    }

    cur_pid = getpid();
    if(lock -> pid != cur_pid){
        return EINVAL;
    }
    
    lock -> pid = -1;
    lock -> lock_state = UNLOCKED;
    thread_wake(&(lock->flag));
    return 0;
}