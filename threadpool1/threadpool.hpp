#include<bits/stdc++.h>
using namespace std;

class Task{
    public:

}

class threadpool{
    public:
        threadpool(int num):stop(false){
            





        }
        ~threadpool(){





            
        }
    private:
    vector<thread> workers; // 线程池中的线程
    queue<Task> tasks; // 任务队列
    condition_variable condition; // 条件变量
    mutex mtx; // 互斥量
    bool stop; // 停止标志
}

