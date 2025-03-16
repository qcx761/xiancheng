#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void cleanup_function(void* arg) {
    printf("Cleaning up: %s\n", (char*)arg);
}

void* thread_function(void* arg) {
    pthread_cleanup_push(cleanup_function, "Resource 1");
    pthread_cleanup_push(cleanup_function, "Resource 2");

    printf("Thread is running...\n");
    for (int i = 0; i < 5; i++) {
        printf("Working... %d\n", i);
        sleep(1);
    }

    pthread_cleanup_pop(1); // 执行清理函数
    pthread_cleanup_pop(1); // 执行清理函数

    return NULL;
}

int main() {
    pthread_t thread;

    // 创建线程
    pthread_create(&thread, NULL, thread_function, NULL);
    sleep(2); // 等待一段时间

    // 取消线程
    pthread_cancel(thread);
    pthread_join(thread, NULL); // 等待线程结束

    printf("Thread has been canceled.\n");
    return 0;
}