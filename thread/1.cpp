#include <bits/stdc++.h>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
using namespace std;

queue<int> queue1;
mutex mtx; // 互斥量
condition_variable condition; // 条件变量
bool done = false; // 标志，用于指示生产是否已完成

void producer() {
    for (int i = 0; i < 10; i++) {
        {
            unique_lock<mutex> lock(mtx);
            queue1.push(i);
            cout << "生产: " << i << endl;
        }
        condition.notify_one(); // 通知消费者
        this_thread::sleep_for(chrono::milliseconds(100)); // 模拟生产时间
    }
    {
        unique_lock<mutex> lock(mtx);
        done = true; // 设置完成标志
    }
    condition.notify_all(); // 通知所有等待的消费者
}

void consumer() {
    while (true) {
        int value;
        {
            unique_lock<mutex> lock(mtx);
            condition.wait(lock, [] { return !queue1.empty() || done; }); // 等待条件
            if (queue1.empty() && done) {
                break; // 如果队列为空且生产完成，退出循环
            }
            value = queue1.front();
            queue1.pop();
        }
        cout << "消费: " << value << endl;     
    }
}

int main() {
    thread t1(producer);
    thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}