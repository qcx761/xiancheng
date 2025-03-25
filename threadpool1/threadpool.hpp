#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP
#include <bits/stdc++.h>
using namespace std;
class threadpool {
    public:

    private:
        vector<thread> workers; // 线程池中的线程
        queue<function<void()>> tasks; // 任务队列
        condition_variable condition; // 条件变量
        mutex mtx; // 互斥量
        bool stop; // 停止标志
    };

#endif