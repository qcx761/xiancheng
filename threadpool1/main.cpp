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

// 定义参数结构体
typedef struct {
    int* array;
    int low;
    int high;
} QuickSortArgs;

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// 函数
int partition(int* array, int low, int high) {
    int pivot = array[high]; // 选择最后一个元素作为基准
    int left = low - 1; // 左指针
    int right = high; // 右指针

    while (1) {
        // 移动左指针，直到找到一个大于基准的元素
        while (array[++left]<pivot){
            if(left>right){
                left--;
                break;
            }
        } 
        // 移动右指针，直到找到一个小于基准的元素
        while (array[--right] > pivot){
            if(left>right){
                right++;
                break;
            }
        } 
        
        if (left ==right) {
            break; // 左右指针相交，跳出循环
        }
        swap(array[left], array[right]); // 交换
    }
    swap(array[left], array[high]); // 将基准元素放到正确的位置
    return left; // 返回基准的索引
}

// 快速排序的实现
void quickSort(void* args) {
    QuickSortArgs* q = (QuickSortArgs*)args;
    int low = q->low;
    int high = q->high;
    int* array = q->array;

    if (low<high){
        int middle = partition(array, low, high);

        // 创建新的参数结构体用于存储子问题的参数
        QuickSortArgs leftArgs = { array, low, middle - 1 };
        QuickSortArgs rightArgs = { array, middle + 1, high };



        // 将左右子问题的排序任务添加到线程池
        Add_task(quickSort, (void*)&leftArgs);
        Add_task(quickSort, (void*)&rightArgs);
    }
}

int main() {
    // 设定线程池    
    ThreadPool(4);
    int arr[] = {38, 27, 43, 3, 9, 82, 10};
    int n = sizeof(arr) / sizeof(arr[0]);

    QuickSortArgs args = { arr, 0, n - 1 };

    // 创建线程池
    ThreadPool pool(4);

    // 启动快速排序
    pool.Add_task(quickSort, (void*)&args);

    // 等待一定时间以确保所有任务完成
    std::this_thread::sleep_for(std::chrono::seconds(2)); 

    // 打印结果
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}