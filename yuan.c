#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>

atomic_int counter = 0; // 原子整型变量

void* increment(void* arg) {
    for (int i = 0; i < 100000; i++) {
        atomic_fetch_add(&counter, 1); // 原子增加
    }
    return NULL;
}

int main() {
    pthread_t threads[10];

    // 创建多个线程
    for (int i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, increment, NULL);
    }

    // 等待所有线程完成
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final counter value: %d\n", counter); // 预期结果为 1000000
    return 0;
}