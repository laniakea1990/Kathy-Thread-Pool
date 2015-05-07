#include "ks_thread_pool.h"

ks_thread_pool_t *ks_thread_pool_init(int thread_num)
{
    int i;
    ks_thread_pool_t *thread_pool;
    thread_pool = (ks_thread_pool_t *)malloc(sizeof(ks_thread_pool_t));
    if(thread_pool == NULL)
    {
        fprintf(stderr, "ks_thread_pool_init error\n");
        return NULL;
    }

    thread_pool->threads = (ks_pthread_t *)malloc(thread_num * sizeof(ks_pthread_t));
    if(thread_pool->threads == NULL)
    {
        fprintf(stderr, "ks_thread_pool_ini(): could not malloc for thread_pool->threads\n");
        return NULL;
    }

    thread_pool->thread_num = thread_num;

    for(i=0; i<thread_num; i++)
    {
        int err;
        thread_pool->threads->id = (pthread_t *)malloc(sizeof(pthread_t));
        printf("create thread\n");
        err = pthread_create(thread_pool->threads->id+i, NULL, thread_do, (void *)thread_pool);
        if(err != 0)
        {
            fprintf(stderr, "pthread_create error\n");
            return NULL;
        }
    }

    thread_pool->job_queue = (ks_job_queue_t *)malloc(sizeof(ks_job_queue_t));
    if(thread_pool->job_queue == NULL)
    {
        fprintf(stderr, "ks_thread_pool_init(): could not malloc for thread_pool->job_queue\n");
        return NULL;
    }

    thread_pool->job_queue->head = (ks_job_queue_element_t *)malloc(sizeof(ks_job_queue_element_t));
    thread_pool->job_queue->head->job = NULL;
    thread_pool->job_queue->head->next = NULL;
    thread_pool->job_num = 0;

    printf("end init\n");
    return thread_pool;
}

/*int ks_job_add(ks_thread_pool_t *thread_pool, )*/
/*{*/

/*}*/



int ks_job_queue_add(ks_job_queue_t *job_queue, void *job)
{
    printf("add job\n");
    ks_job_queue_element_t *job_queue_element;
    job_queue_element = (ks_job_queue_element_t *)malloc(sizeof(ks_job_queue_element_t));
    if(job_queue_element == NULL)
    {
        fprintf(stderr, "ks_job_queue_add(): could not malloc for job_queue_element\n");
        return -1;
    }

    printf("add job 2\n");
    job_queue_element->job = (ks_job_t *)malloc(sizeof(ks_job_t));
    if(job_queue_element->job == NULL)
    {
        fprintf(stderr, "ks_job_queue_add(): could not malloc for job_queue_element->job\n");
        return -1;
    }
    job_queue_element->job->func = job;
    printf("add job3\n");

    job_queue_element->next = job_queue->head->next;
    printf("add job4\n");
    job_queue->head->next = job_queue_element;

    printf("add job success\n");
    
    return 1;
}

void *thread_do(void *thread_pool)
{
    sleep(2);
    void (*func)();
    for(; ;)
    {
        if(((ks_thread_pool_t *)thread_pool)->job_num != 0)
        {
            func =(void (*)())(((ks_thread_pool_t *)thread_pool)->job_queue->head->next->job->func);
            func();
        }
    }
}
