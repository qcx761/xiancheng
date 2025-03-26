#include "threadpool.hpp"
using namespace std;
// 将 q 函数的签名修改为 void*，可以通过参数返回结果
void q(void* arg) {
    int i = *static_cast<int*>(arg); // 从传入的指针中获取参数
    int result = i * i;              // 计算平方
    cout << "Square of " << i << " is " << result << endl; // 输出计算结果
}

int main() {
    Threadpool pool(4); // 创建线程池
    vector<int> taskIds(8); // 存储任务 ID
    int sum = 0; // 结果累加

    for (int i = 0; i < 8; i++) {
        taskIds[i] = i; // 初始化任务 ID
        pool.Add_task(q, &taskIds[i]); // 将任务添加到线程池
    }

    // 等待一些时间来保证所有任务的执行
    // std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0; // 结束程序
}



//Add_task
