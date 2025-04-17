#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <queue>
#include <unistd.h>

#define BUFFER_SIZE 5 // 缓冲区大小

// 定义缓冲区和相关变量
int buffer[BUFFER_SIZE];
int count = 0; // 当前缓冲区中的数据量
pthread_mutex_t mutex;
pthread_cond_t cond_producer;
pthread_cond_t cond_consumer;

// 生产者线程
void* producer(void* arg) {
    for (int i = 0; i < 20; ++i) {
        pthread_mutex_lock(&mutex);
        
        // 如果缓冲区满，生产者等待
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&cond_producer, &mutex);
        }
        
        // 生产数据
        buffer[count] = i;
        printf("Produced: %d\n", i);
        count++;
        
        // 通知消费者
        pthread_cond_signal(&cond_consumer);
        pthread_mutex_unlock(&mutex);
        
        // 模拟生产时间
        sleep(1);
    }
    return NULL;
}

// 消费者线程
void* consumer(void* arg) {
    for (int i = 0; i < 20; ++i) {
        pthread_mutex_lock(&mutex);
        
        // 如果缓冲区空，消费者等待
        while (count == 0) {
            pthread_cond_wait(&cond_consumer, &mutex);
        }
        
        // 消费数据
        int data = buffer[count - 1];
        printf("Consumed: %d\n", data);
        count--;
        
        // 通知生产者
        pthread_cond_signal(&cond_producer);
        pthread_mutex_unlock(&mutex);
        
        // 模拟消费时间
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    // 初始化互斥锁和条件变量
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_producer, NULL);
    pthread_cond_init(&cond_consumer, NULL);

    // 创建生产者和消费者线程
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // 等待线程结束
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // 销毁互斥锁和条件变量
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_producer);
    pthread_cond_destroy(&cond_consumer);

    return 0;
}