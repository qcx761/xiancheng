#include"threadpool.hpp"
using namespace std;
threadpool::threadpool(size_t num):stop(false){
    for(int i=1;i<num;i++){
        this->workers.emplace_back([this]{
            
            while(1){
                function<void()> task;
                {
                unique_lock<mutex> lock(this->mtx);
                bool isempty=this->tasks.empty();
                this->condition.wait(lock,!isempty||this->stop);
                if(stop&&isempty) return;
                task=this->tasks.front();
                this->tasks.pop();
                }
                task();
            }
        });
    }
}

threadpool::~threadpool(){
    {
        unique_lock<mutex> lock(this->mtx);
        this->stop=true;
    }
    this->condition.notify_all();
    for(int j=0;j<this->workers.size();j++){
        workers[j].join();
    }

}



// public:
//         threadpool(size_t numThreads);//threadpool create
//         ~threadpool(); // 释放
    
//         template<typename F>
//         auto enqueue(F&& f) -> future<typename result_of<F()>::type>; // 将任务添加到线程池的任务队列中
    
//     private:
//         vector<thread> workers; // 线程池中的线程
//         queue<function<void()>> tasks; // 任务队列
//         mutex queueMutex; // 互斥量
//         condition_variable condition; // 条件变量
//         bool stop; // 停止标志