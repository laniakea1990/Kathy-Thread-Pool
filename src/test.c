#include <stdio.h>
#include "kathy_thread_pool.h"

void *print(void *arg)
{
    pthread_t tid = pthread_self();
    char c = *(char *)arg;
    printf("thread %d is doing %c job\n", (int)tid, c);
    sleep(1);
    return (void *)0;
}

int main(void)
{
    kathy_thread_pool_t *thread_pool;
    thread_pool = kathy_thread_pool_init(5);
    int i;
    char string[] = "1234567890qwertyuiopasdfghjklzxcvbnm";

    for(i = 0; i<36; i++)
    {
        ktp_job_add(thread_pool, print, (void *)(string+i));
    }

    sleep(10);

    kathy_thread_pool_destroy(thread_pool);

    return 1;
}
