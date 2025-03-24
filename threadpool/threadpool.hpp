#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP
#include <bits/stdc++.h>
using namespace std;
class threadpool {
    public:
        threadpool(size_t numThreads);//threadpool create
        ~threadpool(); // 释放
    
        //template<typename F>
        //auto enqueue(F&& f) -> future<typename result_of<F()>::type>; // 将任务添加到线程池的任务队列中
    
    private:
        vector<thread> workers; // 线程池中的线程
        queue<function<void()>> tasks; // 任务队列
        mutex mtx; // 互斥量
        condition_variable condition; // 条件变量
        bool stop; // 停止标志
    };

#endif


//quenen
//ector
//function



        //lambda 表达式
        // [capture](parameters) -> return_type {
        //     // function body
        // }
        // capture：指定如何捕获外部变量，可以为空或包含变量。
        // parameters：参数列表，类似于函数的参数。
        // -> return_type：可选，指定返回类型（通常可以省略，编译器会进行推导）。
        // {}：函数体。