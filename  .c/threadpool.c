#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_TASKS 100

typedef struct {
    void (*function)(void *arg); // 任务的函数指针
    void *arg;                    // 任务的参数
} Task;

typedef struct {
    pthread_t *threads;         // 工作线程数组
    Task task_queue[MAX_TASKS]; // 任务队列
    int task_count;             // 当前任务数量
    int shutdown;               // 线程池关闭标志
    pthread_mutex_t lock;       // 用于保护任务队列的互斥锁
    sem_t task_sem;             // 任务信号量
} ThreadPool;

ThreadPool *pool;

// 工作线程函数
void *worker(void *arg) {
    while (1) {
        // 先阻塞等待任务
        sem_wait(&pool->task_sem);

        // 加锁获取任务
        pthread_mutex_lock(&pool->lock);

        // 获取任务
        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->lock);
            break;
        }
        
        Task task = pool->task_queue[--pool->task_count];
        pthread_mutex_unlock(&pool->lock);

        // 执行任务
        task.function(task.arg);
    }
    pthread_exit(NULL);
}

// 初始化线程池
ThreadPool *threadpool_init(int num_threads) {
    pool = malloc(sizeof(ThreadPool));
    pool->task_count = 0;
    pool->shutdown = 0;
    pool->threads = malloc(sizeof(pthread_t) * num_threads);

    pthread_mutex_init(&pool->lock, NULL);
    sem_init(&pool->task_sem, 0, 0);

    // 创建工作线程
    for (int i = 0; i < num_threads; ++i) {
        pthread_create(&pool->threads[i], NULL, worker, NULL);
    }
    
    return pool;
}

// 将任务添加到线程池
void threadpool_add_task(void (*function)(void *arg), void *arg) {
    pthread_mutex_lock(&pool->lock);
    
    if (pool->task_count >= MAX_TASKS) {
        printf("Task queue is full!\n");
        pthread_mutex_unlock(&pool->lock);
        return;
    }
    
    // 添加任务到队列
    pool->task_queue[pool->task_count].function = function;
    pool->task_queue[pool->task_count].arg = arg;
    pool->task_count++;
    
    pthread_mutex_unlock(&pool->lock);
    sem_post(&pool->task_sem); // 发布一个信号
}

// 销毁线程池
void threadpool_destroy() {
    pthread_mutex_lock(&pool->lock);
    pool->shutdown = 1; // 设置关闭标志
    pthread_mutex_unlock(&pool->lock);
    
    // 释放所有线程
    for (int i = 0; i < pool->task_count; ++i) {
        sem_post(&pool->task_sem); // 唤醒所有线程
    }
    
    for (int i = 0; i < MAX_TASKS; ++i) {
        pthread_join(pool->threads[i], NULL);
    }
    
    free(pool->threads);
    free(pool);
}

// 测试任务函数
void print_message(void *arg) {
    char *message = (char *)arg;
    printf("%s\n", message);
    sleep(1); // 睡眠用于模拟任务执行时间
}

// 示例用法
int main() {
    ThreadPool *pool = threadpool_init(4); // 创建一个包含4个工作线程的线程池

    // 添加任务到线程池
    for (int i = 0; i < 10; ++i) {
        char *message = malloc(20);
        snprintf(message, 20, "Task %d", i + 1);
        threadpool_add_task(print_message, message);
    }

    sleep(5); // 等待一些时间以便所有任务完成

    threadpool_destroy(); // 销毁线程池
    return 0;
}