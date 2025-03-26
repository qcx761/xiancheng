#include "threadpool.hpp"
using namespace std;
int main(){
    threadpool(4);
    return 0;
}



// void printMessage(void* arg) {
//     char* message = static_cast<char*>(arg); // 将 void* 转换为 char*
//     std::cout << message << std::endl;
// }

// int main() {
//     char message[] = "Hello from a normal function!";
//     Task task(printMessage, message);
//     // 这里可以直接调用 task.function(task.arg) 来执行任务
//     task.function(task.arg);
//     return 0;
// }
//函数传参