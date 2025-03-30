#include "threadpool.hpp"
using namespace std;
int main(){
    threadpool pool(4);
    std::vector<std::future<int>> results;
    
    for (int i = 0; i < 8; ++i) {
        results.emplace_back(pool.enqueue([i] {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return i * i;
        }));
    }
    
    for (auto& result : results) {
        std::cout << result.get() << ' ';
    }
    
    return 0;
}



//enqueue 方法接受一个可调用对象作为参数，这个可调用对象可以是函数指针、lambda 表达式、成员函数指针或任何实现了 operator() 的对象。