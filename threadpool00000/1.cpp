#include <bits/stdc++.h>
using namespace std;
    queue<int> queue1;
    mutex mtx; // 互斥量
    condition_variable condition; // 条件变量
    bool done=false;
void producer(){
    for(int i=0;i<10;i++){
        {
        unique_lock<mutex> lock(mtx);
        queue1.push(i);
        condition.notify_one();
        cout << "p" << i <<endl;
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}
void consumer(){
    while(1){
    unique_lock<mutex> lock(mtx);
    condition.wait(lock,[](){return !queue1.empty();});
    int value=queue1.front();
    queue1.pop();
    cout << "c" << value <<endl;    
    }
}

int main(){
    thread t1(producer);
    thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}