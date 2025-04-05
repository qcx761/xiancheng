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

// int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset); //线程信号掩码，单独处理信号集
// how:
// 指定如何修改信号掩码。可以取以下值之一：
// SIG_BLOCK: 将 set 中指定的信号添加到当前线程的信号掩码中，即“阻塞”这些信号。
// SIG_UNBLOCK: 从当前线程的信号掩码中移除 set 中指定的信号，即“解除对这些信号的阻塞”。
// SIG_SETMASK: 将当前线程的信号掩码替换为 set 中指定的信号集。
// set:
// 指向 sigset_t 类型的信号集的指针，该信号集表示要添加、移除或设置为当前信号掩码的信号。
// 如果 how 参数为 SIG_UNBLOCK，则 set 应包含要解除阻塞的信号；如果进行屏蔽或更改，则应包含新的信号集。
// oldset:
// 指向 sigset_t 类型的变量的指针，用于保存调用前的信号掩码。如果不关心旧的信号掩码，可以将其设置为 NULL。这使得调用者能够在之后恢复到之前的信号掩码。
// 返回值
// 成功时，返回 0；失败时，返回一个非零值并设置 errno 变量以指示错误类型。

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