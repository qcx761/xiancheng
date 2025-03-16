#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* thread_function(void* arg) {
    printf("Thread is running...\n");
    sleep(5);  // 模拟工作
    printf("Thread is exiting...\n");
    return NULL;
}

int main() {
    pthread_t thread;

    // 创建线程
    if (pthread_create(&thread, NULL, thread_function, NULL) != 0) {
        perror("Failed to create thread");
        exit(EXIT_FAILURE);
    }

    sleep(2); // 主线程等待一段时间

    // 取消线程
    pthread_cancel(thread);
    pthread_join(thread, NULL); // 等待线程结束

    printf("Thread has been canceled.\n");
    return 0;
}