#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock; // 声明互斥锁
pthread_cond_t cond;  // 声明条件变量
int ready = 0;        // 条件状态

void *consumer(void *arg) {
    pthread_mutex_lock(&lock); // 加锁
    while (ready == 0) {        // 检查条件
        pthread_cond_wait(&cond, &lock); // 等待条件变量的通知
    }
    printf("Consumer: Ready is now 1\n");
    pthread_mutex_unlock(&lock); // 解锁
    return NULL;
}

void *producer(void *arg) {
    pthread_mutex_lock(&lock); // 加锁
    ready = 1;                 // 更新条件状态
    printf("Producer: Setting ready to 1\n");
    pthread_cond_signal(&cond); // 通知一个等待的线程
    pthread_mutex_unlock(&lock); // 解锁
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // 初始化互斥锁和条件变量
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    //开头一次性初始化
    //pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // 一次性初始化互斥锁
    //pthread_cond_t cond = PTHREAD_COND_INITIALIZER;   // 一次性初始化条件变量

    

    // 创建消费者线程
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    // 创建生产者线程
    pthread_create(&producer_thread, NULL, producer, NULL);

    // 等待线程完成
    pthread_join(consumer_thread, NULL);
    pthread_join(producer_thread, NULL);

    // 销毁互斥锁和条件变量
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}