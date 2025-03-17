static void *threadFunc(void *arg)
{
    int s;
    void *buf = NULL;

    /* 分配内存 */
    buf = malloc(0x10000);
    printf("thread: allocated memory at %p\n", buf);

    /* 锁定互斥锁 */
    s = pthread_mutex_lock(&mtx);
    if (s != 0)
        errExitRV(s, "pthread_mutex_lock");

    /* 将清理函数推入清理栈 */
    pthread_cleanup_push(cleanupHandler, buf);

    /* 等待条件变量 */
    while (glob == 0) {
        s = pthread_cond_wait(&cond, &mtx);
        if (s != 0)
            errExitRV(s, "pthread_cond_wait");
    }

    printf("thread: condition wait loop completed\n");

    /* 弹出清理函数 */
    pthread_cleanup_pop(1);

    return NULL;
}