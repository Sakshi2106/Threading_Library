#define LOCKED 0
#define UNLOCKED 1

typedef struct thread_spinlock{

    //Lock's owner thread pid
    pid_t pid;

    //Lock state 
    int lock_state;

    int flag;

}thread_spinlock;

int thread_spin_init(thread_spinlock *lock);
int thread_spin_destroy(thread_spinlock *lock);
int thread_spin_lock(thread_spinlock *lock);
int thread_spin_unlock(thread_spinlock *lock);