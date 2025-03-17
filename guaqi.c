#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ready = 0;

void* thread_function(void* arg) {
    pthread_mutex_lock(&mutex);
    while (!ready) {
        // 挂起线程，等待条件变量
        pthread_cond_wait(&cond, &mutex);
    }
    printf("Thread is running...\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, NULL);

    sleep(2); // 模拟主线程工作
    pthread_mutex_lock(&mutex);
    ready = 1; // 设定条件
    pthread_cond_signal(&cond); // 唤醒挂起的线程
    pthread_mutex_unlock(&mutex);

    pthread_join(thread, NULL); // 等待线程结束
    return 0;
}