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

template<typename F,class... Args>
auto enqueue(F&& f,Args&&... args) -> future<invoke_result_t<F,Args...>>{
    using return_type=invoke_result_t<F,Args...>;
     // 绑定函数和参数，创建packaged_task
    auto task=make_shared<packaged_task<return_type()>>(bind(forward<F>(f),forward<Args>(args)...);
    future<return_type> res=task->get_future();
    
    );
}


// template<typename F>
// auto ThreadPool::enqueue(F&& f) -> std::future<std::invoke_result_t<F>>
// {
//     using return_type = std::invoke_result_t<F>;

//     // 1. 创建 packaged_task（已绑定可调用对象）
//     auto task = std::make_shared<std::packaged_task<return_type()>>(
//         std::forward<F>(f)
//     );

//     // 2. 获取 future
//     std::future<return_type> res = task->get_future();

//     // 3. 临界区（加锁入队）
//     {
//         std::unique_lock<std::mutex> lock(queue_mutex);
//         if(stop) {
//             throw std::runtime_error("enqueue on stopped ThreadPool");
//         }
//         tasks.emplace([task] { (*task)(); }); // 包装为 void() 类型
//     }

//     // 4. 唤醒一个工作线程
//     condition.notify_one();

//     return res;
// }


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