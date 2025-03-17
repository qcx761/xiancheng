// 常用函数
// sigemptyset：初始化一个信号集，使其不包含任何信号。
// c

// void sigemptyset(sigset_t *set);
// sigaddset：向信号集中添加一个信号。
// c

// int sigaddset(sigset_t *set, int signum);
// sigprocmask：用于修改当前进程的信号屏蔽字。
// c

// int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
// sigpending：检查当前进程中等待处理的信号。
// c

// int sigpending(sigset_t *set);
// 示例代码
// 以下是一个示例，展示如何使用信号掩码在多线程程序中屏蔽信号：


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

void* thread_function(void* arg) {
    sigset_t *set = (sigset_t *)arg;
    int sig;

    // 等待信号
    printf("Thread waiting for signals...\n");
    if (sigwait(set, &sig) != 0) {
        perror("sigwait failed");
        return NULL;
    }

    printf("Thread received signal: %d\n", sig);
    return NULL;
}

int main() {
    pthread_t thread;
    sigset_t set;

    // 初始化信号集
    sigemptyset(&set);
    sigaddset(&set, SIGINT);  // 添加 SIGINT 信号
    sigaddset(&set, SIGTERM); // 添加 SIGTERM 信号

    // 将信号集设置为线程的屏蔽信号
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    // 创建线程
    pthread_create(&thread, NULL, thread_function, &set);

    // 主线程等待一段时间
    sleep(5);
    printf("Sending SIGINT to the process\n");
    raise(SIGINT); // 向进程发送 SIGINT 信号

    pthread_join(thread, NULL); // 等待线程结束
    return 0;
}