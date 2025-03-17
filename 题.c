#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_PROCESSES 3
#define NUM_THREADS 2

// 共享资源
int counter = 0;
pthread_mutex_t mutex;

// 线程函数
void* thread_function(void* arg) {
    int thread_id = *(int*)arg;

    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        printf("Process %d, Thread %d: Counter = %d\n", getpid(), thread_id, counter);
        pthread_mutex_unlock(&mutex);
        sleep(1); // 模拟一些工作
    }
    return NULL;
}

// 进程函数
void create_process() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i; // 线程 ID
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    // 等待所有线程结束
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    // 初始化互斥锁
    pthread_mutex_init(&mutex, NULL);

    // 创建多个子进程
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // 子进程
            printf("Starting process %d\n", getpid());
            create_process();
            exit(EXIT_SUCCESS); // 子进程完成后退出
        }
    }

    // 等待所有子进程结束
    for (int i = 0; i < NUM_PROCESSES; i++) {
        wait(NULL);
    }

    // 清理资源
    pthread_mutex_destroy(&mutex);
    printf("Final Counter Value: %d\n", counter);
    return 0;
}