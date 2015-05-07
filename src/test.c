#include <stdio.h>
#include "ks_thread_pool.h"
#include <unistd.h>

void print()
{
    printf("hello world!\n");
}

void printsome()
{
    printf("bitches! go fuck yourself\n");
}

int main(void)
{
    ks_thread_pool_t *thread_pool;
    thread_pool = ks_thread_pool_init(1);

    printf("start\n");
    if(ks_job_queue_add(thread_pool->job_queue, (void *)printsome) < 0)
    {
        fprintf(stderr, "ks_job_queue_add error\n");
        return 0;
    }
    printf("main add job\n");
    thread_pool->job_num++;
    sleep(3);
    return 1;
}
