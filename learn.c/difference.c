// 在 C 和 C++ 中创建线程的方式有所不同，主要体现在使用的库和语言特性上。以下是两者之间的主要区别：

// 1. 语言及库
// C: 通常使用 POSIX 线程（pthread）库来创建和管理线程。
// C++: C++11 及后续版本引入了标准线程库（<thread>），允许以更自然和面向对象的方式创建和管理线程。
// 2. 创建线程的语法
// C 中的线程创建
// 在 C 中，使用 pthread_create() 函数创建线程。代码示例：

// c

// 复制
// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>

// void* thread_function(void* arg) {
//     printf("Hello from C thread!\n");
//     return NULL;
// }

// int main() {
//     pthread_t thread;
//     if (pthread_create(&thread, NULL, thread_function, NULL) != 0) {
//         perror("Failed to create thread");
//         return 1;
//     }
    
//     pthread_join(thread, NULL); // 等待线程结束
//     return 0;
// }
// C++ 中的线程创建
// 在 C++ 中，使用 std::thread 类来创建线程。代码示例：

// cpp

// 复制
// #include <iostream>
// #include <thread>

// void thread_function() {
//     std::cout << "Hello from C++ thread!" << std::endl;
// }

// int main() {
//     std::thread thread(thread_function);
    
//     thread.join(); // 等待线程结束
//     return 0;
// }
// 3. 参数传递
// C: 使用 void* 类型传递参数，通常通过强制类型转换来处理。
// C++: 直接使用函数指针、lambda 表达式或类成员函数，可以使用更强的类型检查。
// C 中参数传递示例
// c

// 复制
// void* thread_function(void* arg) {
//     int id = *((int*)arg);
//     printf("Thread ID: %d\n", id);
//     return NULL;
// }
// C++ 中参数传递示例
// 使用 lambda 表达式：

// cpp

// 复制
// int main() {
//     int id = 1;
//     std::thread thread([id]() {
//         std::cout << "Thread ID: " << id << std::endl;
//     });
    
//     thread.join(); // 等待线程结束
//     return 0;
// }
// 4. 线程同步和互斥
// C: 通过 pthread 库提供的 mutex 和条件变量进行同步。
// C++: C++11 引入了 std::mutex、std::condition_variable 等类，提供了更加方便和安全的机制。
// C 中的互斥示例
// c

// 复制
// #include <pthread.h>

// pthread_mutex_t lock;

// void* thread_function(void* arg) {
//     pthread_mutex_lock(&lock);
//     // 关键区
//     pthread_mutex_unlock(&lock);
//     return NULL;
// }
// C++ 中的互斥示例
// cpp

// 复制
// #include <iostream>
// #include <thread>
// #include <mutex>

// std::mutex mtx;

// void thread_function() {
//     std::lock_guard<std::mutex> lock(mtx); // 自动锁定和解锁
//     // 关键区
// }
// 5. 异常处理
// C: 处理线程中的错误通常依赖于线程函数的返回值。
// C++: 可以使用异常处理机制（try-catch）来处理线程中的异常。
// 6. 总结
// 库支持: C 使用 pthread 库，C++ 使用标准线程库。
// 语法和特性: C 语言的线程创建和管理较为底层，而 C++ 提供了更高级的封装和面向对象的支持。
// 参数传递: C 采用 void* 类型，而 C++ 允许更强类型的传递，包括 lambda 和函数对象。
// 同步机制: C 的互斥体和条件变量比 C++ 中的类更为复杂，不具备 RAII 特性。
// 异常处理: C++ 提供了更好的异常管理机制。
// 总体而言，C++ 中的线程创建和管理更为现代化、简便和安全，更符合 C++ 的设计理念，能够充分利用面向对象的特性。