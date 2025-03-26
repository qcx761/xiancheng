#include <iostream>
#include <pthread.h>
#include <vector>
#include <queue>
#include <functional>
#include <semaphore.h>
#include <unistd.h>

class Task {
public:
    std::function<void(void*)> function; // 任务的函数指针
    void* arg; // 任务的参数

    Task(std::function<void(void*)> func, void* argument)
        : function(func), arg(argument) {}
};

class ThreadPool {
private:
    std::vector<pthread_t> threads; // 工作线程数组
    std::queue<Task> taskQueue; // 任务队列
    pthread_mutex_t lock; // 用于保护任务队列的互斥锁
    sem_t taskSem; // 任务信号量
    bool shutdown; // 线程池关闭标志
    int numThreads; // 线程数量

    // 工作线程函数
    static void* worker(void* arg) {
        ThreadPool* pool = static_cast<ThreadPool*>(arg);
        
        while (true) {
            // 先阻塞等待任务
            sem_wait(&pool->taskSem);

            pthread_mutex_lock(&pool->lock);
            if (pool->shutdown && pool->taskQueue.empty()) {
                pthread_mutex_unlock(&pool->lock);
                break;
            }

            if (!pool->taskQueue.empty()) {
                Task task = pool->taskQueue.front();
                pool->taskQueue.pop();
                pthread_mutex_unlock(&pool->lock);

                // 执行任务
                task.function(task.arg);
            } else {
                pthread_mutex_unlock(&pool->lock);
            }
        }
        pthread_exit(NULL);
    }

public:
    ThreadPool(int numThreads) : shutdown(false), numThreads(numThreads) {
        pthread_mutex_init(&lock, NULL);
        sem_init(&taskSem, 0, 0);
        threads.resize(numThreads);

        // 创建工作线程
        for (int i = 0; i < numThreads; ++i) {
            pthread_create(&threads[i], NULL, worker, this);
        }
    }

    ~ThreadPool() {
        pthread_mutex_lock(&lock);
        shutdown = true; // 设置关闭标志
        pthread_mutex_unlock(&lock);
        
        // 释放所有线程
        for (int i = 0; i < numThreads; ++i) {
            sem_post(&taskSem); // 唤醒所有线程
        }

        for (auto& thread : threads) {
            pthread_join(thread, NULL);
        }

        pthread_mutex_destroy(&lock);
        sem_destroy(&taskSem);
    }

    void addTask(std::function<void(void*)> func, void* arg) {
        pthread_mutex_lock(&lock);
        taskQueue.push(Task(func, arg)); // 添加任务到队列
        pthread_mutex_unlock(&lock);
        sem_post(&taskSem); // 发布一个信号
    }
};

// 测试任务函数
void printMessage(void* arg) {
    char* message = static_cast<char*>(arg);
    std::cout << message << std::endl;
    sleep(1); // 模拟任务执行时间
}