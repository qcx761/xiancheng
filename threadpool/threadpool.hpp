#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP
#include <bits/stdc++.h>
using namespace std;
class ThreadPool {
    public:
        ThreadPool(size_t numThreads);//threadpool create
        ~ThreadPool(); // 释放
    
        template<typename F>
        auto enqueue(F&& f) -> future<typename result_of<F()>::type>; // 将任务添加到线程池的任务队列中
    
    private:
        vector<thread> workers; // 线程池中的线程
        queue<function<void()>> tasks; // 任务队列
        mutex queueMutex; // 互斥量
        condition_variable condition; // 条件变量
        bool stop; // 停止标志
    };

#endif


//quenen
//ector
//function