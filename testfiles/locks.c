#include <stdio.h>
#include <sys/types.h>
#include "thread.h"
#include <sys/time.h>
#include <sys/types.h>

#define LOOPS 10000000

int list[LOOPS];
int idx = 0;
int cs1 = 0, cs2 = 0;

thread_spinlock spinlock;



void *consumer(void *ptr)
{
    printf("Consumer TID %lu\n", (unsigned long)gettid());

    while (1)
    {
        thread_spin_lock(&spinlock);
        if (idx >= LOOPS)
        {
            thread_spin_unlock(&spinlock);
            break;
        }
        list[idx++] += 1;
        thread_spin_unlock(&spinlock);
        if ((int*)ptr == 1)
            cs1++;
        else
            cs2++;
    }
    return NULL;
}

int main()
{
    int i, lo_cnt = 0, hi_cnt = 0;
    thread_tcb thr1, thr2;
    struct timeval tv1, tv2;

    thread_spin_init(&spinlock);

    // Creating the list content...
    for (i = 0; i < LOOPS; i++)
        list[i] = 0;

    // Measuring time before starting the threads...
    gettimeofday(&tv1, NULL);
    thread_create(&thr1, consumer, (void *)1);
    thread_create(&thr2, consumer, (void *)2);

    thread_join(thr1, NULL);
    thread_join(thr2, NULL);

    // Measuring time after threads finished...
    gettimeofday(&tv2, NULL);

    if (tv1.tv_usec > tv2.tv_usec)
    {
        tv2.tv_sec--;
        tv2.tv_usec += 1000000;
    }

    for (i = 0; i < LOOPS; i++)
    {
        if (list[i] == 0)
        {
            lo_cnt++;
            printf("lo:%d ", i);
        }
        else if (list[i] > 1)
        {
            hi_cnt++;
            printf("hi:%d ", i);
        }
    }
    printf("lo_cnt=%d hi_cnt=%d cs1=%d cs2=%d\n", lo_cnt, hi_cnt, cs1, cs2);

    printf("Time: %ld.%ld sec\n", tv2.tv_sec - tv1.tv_sec, tv2.tv_usec - tv1.tv_usec);
    return 0;
}