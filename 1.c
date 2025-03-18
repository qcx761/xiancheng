#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // 一次性初始化互斥锁
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;   // 一次性初始化条件变量





sigemptyset(&set);
sigaddset(&set, SIGINT);
pthread_sigmask(SIG_BLOCK, &set, NULL);