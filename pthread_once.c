#include <pthread.h>
#include <stdio.h>

pthread_once_t once_control = PTHREAD_ONCE_INIT;

void initialize() {
    printf("Initialization done.\n");
}

void* thread_function(void* arg) {
    pthread_once(&once_control, initialize);
    return NULL;
}

int main() {
    pthread_t threads[5];

    // 创建多个线程
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }

    // 等待所有线程完成
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}