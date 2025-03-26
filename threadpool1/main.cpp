#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <tuple>
#include <memory>
#include <future>
#include "threadpool.hpp" // 确保这包含您的线程池实现

using namespace std;

// 合并两个已排序的部分
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1);
    vector<int> R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// 归并排序的普通函数
void mergeSort(void* params) {
    auto* args = static_cast<tuple<vector<int>*, int, int>*>(params);
    vector<int>& arr = *get<0>(*args);
    int left = get<1>(*args);
    int right = get<2>(*args);

    if (left < right) {
        int mid = left + (right - left) / 2;

        // 创建参数元组用于子任务
        auto leftArgs = new tuple<vector<int>*, int, int>(&arr, left, mid);
        auto rightArgs = new tuple<vector<int>*, int, int>(&arr, mid + 1, right);

        // 使用线程池添加子任务
        ThreadPool* pool = new ThreadPool(4); // 创建线程池

        pool->Add_task([](void* arg) {
            mergeSort(arg); // 调用 mergeSort
        }, (void*)leftArgs);

        pool->Add_task([](void* arg) {
            mergeSort(arg); // 调用 mergeSort
        }, (void*)rightArgs);

        // 合并已排序部分
        // 注意这里不能在上面添加任务后直接合并
        // 在所有子任务完成之后执行合并
        // 这部分需要重新考虑如何同步。

        // 可以使用同步机制，例如在 Add_task 类中添加等待机制
    }
}

int main() {
    vector<int> arr = {38, 27, 43, 3, 9, 82, 10};

    // 创建参数元组
    tuple<vector<int>*, int, int> params(&arr, 0, arr.size() - 1);

    // 创建线程池
    ThreadPool pool(4);

    // 调用归并排序
    pool.Add_task(mergeSort, (void*)&params);

    // 等待一些时间确保所有任务的执行
    std::this_thread::sleep_for(std::chrono::seconds(1)); // 确保任务完成（需要更好的同步机制）

    // 打印结果
    for (const auto& num : arr) {
        cout << num << " ";
    }
    cout << endl;

    return 0; // 结束程序
}