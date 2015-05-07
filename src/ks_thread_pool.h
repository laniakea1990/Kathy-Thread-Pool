#ifndef _KS_THREAD_POOL_
#define _KS_THREAD_POOL_

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <pthread.h>

typedef struct{
    pthread_t *id;
}ks_pthread_t;

typedef struct{
    void *func;
}ks_job_t;

struct ks_job_queue_element_s;

typedef struct ks_job_queue_element_s{
    ks_job_t *job;
    struct ks_job_queue_element_s *next;
}ks_job_queue_element_t;

typedef struct{
    ks_job_queue_element_t *head; 
}ks_job_queue_t;

typedef struct{
    ks_pthread_t *threads;
    int thread_num;
    ks_job_queue_t *job_queue;
    int job_num;
}ks_thread_pool_t;


ks_thread_pool_t *ks_thread_pool_init(int thread_num);

int ks_job_add(ks_thread_pool_t *thread_pool, ks_job_t *job);

int ks_job_del(ks_thread_pool_t *thread_pool, ks_job_t *job);

int ks_job_queue_add(ks_job_queue_t *job_queue, void *job);

int ks_job_queue_del(ks_job_queue_t *job_queue);

void *thread_do(void *thread_pool);

#endif
