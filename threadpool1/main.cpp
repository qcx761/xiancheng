#include <bits/stdc++.h>
#include "threadpool.hpp" // 请确保这个头文件包含了您的线程池实现

using namespace std;

std::mutex coutMutex; // 定义一个互斥量以保护 std::cout 的访问

// 修改后的任务函数
void taskFunction(void* arg) {
    int i = *static_cast<int*>(arg); // 从传入的指针中获取参数
    int result = i * i;              // 计算平方

    // 使用锁来保护输出
    {
        std::lock_guard<std::mutex> lock(coutMutex); // 锁定输出
        cout << "Square of " << i << " is " << result << endl; // 输出计算结果
    }
}

int main() {
    ThreadPool pool(4); // 创建线程池，指定线程数量为 4
    vector<int> taskIds(8); // 存储任务 ID

    // 初始化任务 ID 和添加到线程池
    for (int i = 0; i < 8; i++) {
        taskIds[i] = i; // 初始化任务 ID
        pool.Add_task(taskFunction, &taskIds[i]); // 将任务添加到线程池
    }

    // 等待一些时间来保证所有任务的执行完成
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0; // 结束程序
}