#ifndef _KTP_THREAD_POOL_
#define _KTP_THREAD_POOL_


#include "ktp_linux_config.h" 


typedef struct{ /* 封装线程的状态 */
    pthread_t *tid;
}ktp_thread_t;

typedef struct ktp_job_s ktp_job_t; /* 封装任务的状态 */
typedef struct ktp_job_s{
    void *(*func)(void *arg);
    void *arg;
    ktp_job_t *next;
    ktp_job_t *prev;
}ktp_job_t;

typedef struct{ /* 任务队列 */
    ktp_job_t *head; 
    int job_num;
    pthread_mutex_t ktp_job_queue_lock;
    sem_t ktp_job_queue_sem;
}ktp_job_queue_t;

typedef struct{ /* 线程池 */
    ktp_thread_t *threads;
    int thread_num;
    ktp_job_queue_t *job_queue;
    pthread_mutex_t kathy_thra_pool_lock;
}kathy_thread_pool_t;


kathy_thread_pool_t *kathy_thread_pool_init(int thread_num);

int kathy_thread_pool_destroy(kathy_thread_pool_t *thread_pool);

int ktp_job_add(kathy_thread_pool_t *thread_pool, void *(*func)(void *arg), void *arg);

int ktp_job_del(kathy_thread_pool_t *thread_pool, ktp_job_t *job);


void *thread_do(void *arg);


ktp_job_queue_t *ktp_job_queue_init();

int ktp_job_queue_destroy(ktp_job_queue_t *job_queue);

int ktp_job_queue_push(ktp_job_queue_t *job_queue, ktp_job_t *job);

ktp_job_t *ktp_job_queue_pop(ktp_job_queue_t *job_queue);

#endif
