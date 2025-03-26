#include<bits/stdc++.h>
using namespace std;

class Task{
    public:
        function<void(void*)> function;
        void *arg;
        Task(std::function<void(void*)> func,void* args):function(func),arg(args){};
};

class threadpool{
    public:
        threadpool(int num):stop(false){
            for(int i=0;i<num;i++){
                this->workers.emplace_back([this]{
                    while(1){
                        Task task(nullptr,nullptr);
                        {
                        unique_lock<mutex> lock(this->mtx);
                        this->condition.wait(lock,[this]{return !this->tasks.empty()||this->stop;});
                        if(stop&&this->tasks.empty()) return;
                        task=std::move(this->tasks.front());
                        this->tasks.pop();
                        }
                        if(task.function){
                            task.function(task.arg);// 执行函数
                        }
                    }
                });
            }
        }
        ~threadpool(){






        }
    void Add_task(function<void(void*)> func,void* args){
        {
            unique_lock<mutex> lock(this->mtx);
            tasks.emplace(Task(func,args))
        }
        condition.notify_one(); // 通知一个线程
    }
    private:
    vector<thread> workers; // 线程池中的线程
    queue<Task> tasks; // 任务队列
    condition_variable condition; // 条件变量
    mutex mtx; // 互斥量
    bool stop; // 停止标志
};

