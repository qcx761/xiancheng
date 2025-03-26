#include <iostream>
#include <vector>
#include <algorithm>
#include <future>
#include "threadpool.hpp" // 确保正确包含线程池的头文件

using namespace std;

// 合并函数，用于合并已排序的部分
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(arr.begin() + left, arr.begin() + mid + 1);
    vector<int> R(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

// 多线程排序函数
void parallel_sort(vector<int>& arr, int left, int right, threadpool& pool) {
    if (left < right) {
        int mid = left + (right - left) / 2; // 计算中间点

        // 向线程池提交排序任务
        auto left_sort_future = pool.enqueue([&arr, left, mid, &pool]() {
            parallel_sort(arr, left, mid, pool);
        });
        auto right_sort_future = pool.enqueue([&arr, mid+1, right, &pool]() {
            parallel_sort(arr, mid + 1, right, pool);
        });

        // 等待两个子任务完成
        left_sort_future.get();
        right_sort_future.get();

        // 合并已排序的部分
        merge(arr, left, mid, right);
    }
}

int main() {
    vector<int> arr = {38, 27, 43, 3, 9, 82, 10}; // 待排序的数组
    threadpool pool(4); // 创建线程池

    // 开始多线程排序
    parallel_sort(arr, 0, arr.size() - 1, pool);
    
    // 输出排序结果
    for (const auto& num : arr) {
        cout << num << ' ';
    }

    return 0;
}