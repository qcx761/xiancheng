#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP
#include <bits/stdc++.h>
using namespace std;
class threadpool {
    public:
        threadpool(size_t num);//线程池创建
        ~threadpool(); // 释放
    
        template<typename F,class... Args> // 可变参数的 enqueue 版本
        auto enqueue(F&& f,Args&&... args) -> future<invoke_result_t<F,Args...>>; // 创建任务队列
    private:
        vector<thread> workers; // 线程池中的线程
        queue<function<void()>> tasks; // 任务队列
        condition_variable condition; // 条件变量
        mutex mtx; // 互斥量
        bool stop; // 停止标志
    };

#endif


//queue
//vector
//function



        //lambda 表达式
        // [capture](parameters) -> return_type {
        //     // function body
        // }
        // capture：指定如何捕获外部变量，可以为空或包含变量。
        // parameters：参数列表，类似于函数的参数。
        // -> return_type：可选，指定返回类型（通常可以省略，编译器会进行推导）。
        // {}：函数体。

//enqueue 方法中使用了 lambda 表达式作为任务，lambda 表达式的类型是局部的（即在 main 函数中定义的），所以在 enqueue 方法中引用它时会导致编译器无法推导它的返回类型。
//???
//确保了 enqueue 函数的定义直接在 threadpool.hpp 头文件中。
// 由于模板函数的实例化需要在编译时完成，所有模板的实现都必须在其声明处可见。因此，我将 enqueue 函数的实现搬到了头文件中，而不是放在 .cpp 文件中。