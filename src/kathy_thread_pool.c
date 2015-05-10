#include "kathy_thread_pool.h"
#include <unistd.h>

kathy_thread_pool_t *kathy_thread_pool_init(int thread_num)
{
    int i;
    kathy_thread_pool_t *thread_pool;
    ktp_thread_t *threads;
    thread_pool = (kathy_thread_pool_t *)malloc(sizeof(kathy_thread_pool_t));
    if(thread_pool == NULL)
    {
        fprintf(stderr, "ks_thread_pool_init error\n");
        return NULL;
    }

    thread_pool->threads = (ktp_thread_t *)malloc(thread_num * sizeof(ktp_thread_t));
    if(thread_pool->threads == NULL)
    {
        fprintf(stderr, "ks_thread_pool_ini(): could not malloc for thread_pool->threads\n");
        free(thread_pool);
        return NULL;
    }

    thread_pool->thread_num = thread_num;
    threads = thread_pool->threads;

    thread_pool->job_queue = ktp_job_queue_init();

    for(i=0; i<thread_num; i++)
    {
        (threads+i)->tid = (pthread_t *)malloc(sizeof(pthread_t));
        if(pthread_create((threads+i)->tid, NULL, thread_do, (void *)thread_pool))
        {
            fprintf(stderr, "pthread_create error\n");
            free(thread_pool);
            return NULL;
        }
    }

    return thread_pool;
}

int kathy_thread_pool_destroy(kathy_thread_pool_t *thread_pool)
{
    ktp_thread_t *threads = thread_pool->threads;
    ktp_job_queue_t *job_queue = thread_pool->job_queue;
    ktp_job_queue_destroy(job_queue);
    free(threads);
    free(thread_pool);
    return 1;
}

int ktp_job_add(kathy_thread_pool_t *thread_pool, void *(*func)(void *arg), void *arg)
{
    ktp_job_queue_t *job_queue = thread_pool->job_queue;
    ktp_job_t *job;
    int result;

    job = (ktp_job_t *)malloc(sizeof(ktp_job_t));
    if(job == NULL)
    {
        fprintf(stderr, "ktp_job_add(): could not malloc for job\n");
        return -1;
    }
    job->func = func;
    job->arg = arg;
    job->next = NULL;
    job->prev = NULL;

    if(ktp_job_queue_push(job_queue, job) > 0)
    {
        result = 1;
    }
    else
    {
        result = -1;
    }
    return result;
}


void *thread_do(void *thread_pool)
{
    ktp_job_queue_t *job_queue = ((kathy_thread_pool_t *)thread_pool)->job_queue;
    ktp_job_t *job;
    void *(*func)(void *arg);
    void *arg;
    while(1)
    {
        sem_wait(&job_queue->ktp_job_queue_sem);
        job = ktp_job_queue_pop(job_queue);
        if(job != NULL)
        {
            func = job->func;
            arg = job->arg;
            func(arg);
            free(job);
        }
    }
}


ktp_job_queue_t *ktp_job_queue_init()
{
    ktp_job_queue_t *job_queue;
    job_queue = (ktp_job_queue_t *)malloc(sizeof(ktp_job_queue_t));
    if(job_queue == NULL)
    {
        fprintf(stderr, "ks_thread_pool_init(): could not malloc for thread_pool->job_queue\n");
        return NULL;
    }
    job_queue->head = NULL;
    job_queue->job_num = 0;

    if(pthread_mutex_init(&(job_queue->ktp_job_queue_lock), NULL) != 0)
    {
        fprintf(stderr, "kathy_thread_pool_init(): pthread_mutex_init failed!\n");
        free(job_queue);
        return NULL;
    }

    if(sem_init(&(job_queue->ktp_job_queue_sem), 0, 0))
    {
        fprintf(stderr, "kathy_thread_pool_init(): sem_init failed!\n");
        pthread_mutex_destroy(&(job_queue->ktp_job_queue_lock));
        free(job_queue);
        return NULL;
    }

    return job_queue;
}

int ktp_job_queue_destroy(ktp_job_queue_t *job_queue)
{
    sem_destroy(&job_queue->ktp_job_queue_sem);
    pthread_mutex_destroy(&job_queue->ktp_job_queue_lock);
    free(job_queue);
    return 1;
}

int ktp_job_queue_push(ktp_job_queue_t *job_queue, ktp_job_t *job)
{
    pthread_mutex_lock(&job_queue->ktp_job_queue_lock);
    job->next = job_queue->head;
    job_queue->head = job;
    job_queue->job_num++;
    pthread_mutex_unlock(&job_queue->ktp_job_queue_lock);
    sem_post(&job_queue->ktp_job_queue_sem);
    return 1;
}

ktp_job_t *ktp_job_queue_pop(ktp_job_queue_t *job_queue)
{
    ktp_job_t *job;
    pthread_mutex_lock(&job_queue->ktp_job_queue_lock);
    if(job_queue->job_num <= 0)
    {
        job = NULL;
    }
    else
    {
        job = job_queue->head;
        job_queue->head = job_queue->head->next;
        job_queue->job_num--;
    }
    pthread_mutex_unlock(&job_queue->ktp_job_queue_lock);
    return job;
}
