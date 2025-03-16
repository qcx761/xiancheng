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


//异步取消

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* thread_function(void* arg) {
    printf("Thread is running...\n");

    // 设置取消类型为异步取消
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    // 模拟长时间运行的任务
    for (int i = 0; i < 10; i++) {
        printf("Working... %d\n", i);
        sleep(1); // 模拟休眠

        // 每次迭代都可以被取消
        pthread_testcancel(); 
    }

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

    sleep(3); // 等待一段时间

    // 取消线程
    pthread_cancel(thread);
    pthread_join(thread, NULL); // 等待线程结束

    printf("Thread has been canceled.\n");
    return 0;
}