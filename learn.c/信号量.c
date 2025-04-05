// int sem_init(sem_t *sem, int pshared, unsigned int value);
// sem: 信号量的指针。
// pshared: 如果设置为 0，信号量仅用于线程之间的同步；如果设置为 1，信号量可以用于进程间的同步。
// value: 初始化时的信号量计数。
// int sem_wait(sem_t *sem);
// 减少信号量的值。如果信号量的值为 0，调用线程将被阻塞。
// int sem_post(sem_t *sem);
// 增加信号量的值，可能会唤醒那些在该信号量上等待的线程。
// int sem_destroy(sem_t *sem);
// 销毁信号量，释放占用的资源。

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 5

// 共享资源
int counter = 0;
// 信号量
sem_t sem;

void* thread_func(void* arg) {
    // 等待信号量
    sem_wait(&sem);
    // 临界区
    int temp = counter;
    // 模拟处理时间
    sleep(1);
    counter = temp + 1;
    printf("Thread %ld incremented counter to %d\n", (long)arg, counter);
    // 发布信号量
    sem_post(&sem);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // 初始化信号量，初始值为1
    sem_init(&sem, 0, 1);
    
    // 创建多个线程
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void*)i);
    }
    
    // 等待所有线程结束
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // 销毁信号量
    sem_destroy(&sem);
    
    printf("Final counter value: %d\n", counter);
    return 0;
}