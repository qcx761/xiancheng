#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <memory>
#include "threadpool.hpp" // 确保这包含您的线程池实现

using namespace std;

// 设定线程池
ThreadPool pool(4);

// 定义参数结构体
struct QuickSortArgs {
    int* array;
    int low;
    int high;
};

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// 分区函数
int partition(int* array, int low, int high) {
    int pivot = array[low];
    int i = low + 1;
    int j = high;

    while (true) {
        while (i <= high && array[i] <= pivot) {
            i++;
        }
        while (j >= low && array[j] > pivot) {
            j--;
        }
        if (i >= j) {
            break; // 找到指针交叉，退出循环
        }
        swap(array[i], array[j]); // 交换
    }
    swap(array[low], array[j]); // 将基准放到正确的位置
    return j; // 返回基准的索引
}

// 快速排序的实现
void quickSort(void* args) {
    QuickSortArgs* q = static_cast<QuickSortArgs*>(args);
    int low = q->low;
    int high = q->high;
    int* array = q->array;

    if (low < high) {
        int middle = partition(array, low, high);

        // 使用智能指针来管理内存
        if (middle > low) {
            auto leftArgs = new QuickSortArgs{array, low, middle - 1};
            pool.Add_task(quickSort, leftArgs);
        }
        if (middle < high) {
            auto rightArgs = new QuickSortArgs{array, middle + 1, high};
            pool.Add_task(quickSort, rightArgs);
        }
    }

    delete q; // 清理传递的参数
}

int main() {
    int arr[] = {38, 27, 43, 3, 9, 82, 10, 23, 15, 90, 32};
    int n = sizeof(arr) / sizeof(arr[0]);

    QuickSortArgs* args = new QuickSortArgs{arr, 0, n - 1};

    // 启动快速排序
    pool.Add_task(quickSort, args);

    // 确保所有任务完成
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 打印结果
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}