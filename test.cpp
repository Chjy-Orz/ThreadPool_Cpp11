//
// Created by autocars on 2023/5/15.
//


#include <iostream>
#include "ThreadPool.h"
using namespace std;

void testThreadPool(){
    ThreadPool threadPool(2);
//    threadPool.Start(2);
    /**
     * 线程功能：生产者 -- 向线程池任务队列中添加任务：输出 同步层线程x的线程ID:x
     */
    thread thd1([&threadPool]{
        for (int i = 0; i < 10; ++i) {
            auto thid = this_thread::get_id();
            threadPool.addTask([thid]{
                cout << "同步层线程1的线程ID：" << thid << endl;
            });
        }
    });
    thread thd2([&threadPool]{
        for (int i = 0; i < 10; ++i) {
            auto thid = this_thread::get_id();
            threadPool.addTask([thid]{
                cout << "同步层线程2的线程ID：" << thid << endl;
            });
        }
    });

    this_thread::sleep_for(chrono::seconds(2));
    getchar();
    threadPool.Stop();
    thd1.join();
    thd2.join();
}

int main() {
    testThreadPool();
    return 0;
}