#include"threadpool.hpp"
using namespace std;
threadpool::threadpool(size_t numThreads):done(false){

}
int main(){
    
    
    return 0;
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