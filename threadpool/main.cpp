#include "threadpool.hpp"
using namespace std;
int main(){
    threadpool pool(4); //调用构造函数创建4个线程
    vector<future<int>> results;
}











//enqueue 方法接受一个可调用对象作为参数，这个可调用对象可以是函数指针、lambda 表达式、成员函数指针或任何实现了 operator() 的对象。