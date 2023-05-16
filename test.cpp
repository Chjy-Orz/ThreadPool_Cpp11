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
     * �̹߳��ܣ������� -- ���̳߳�������������������� ͬ�����߳�x���߳�ID:x
     */
    thread thd1([&threadPool]{
        for (int i = 0; i < 10; ++i) {
            auto thid = this_thread::get_id();
            threadPool.addTask([thid]{
                cout << "ͬ�����߳�1���߳�ID��" << thid << endl;
            });
        }
    });
    thread thd2([&threadPool]{
        for (int i = 0; i < 10; ++i) {
            auto thid = this_thread::get_id();
            threadPool.addTask([thid]{
                cout << "ͬ�����߳�2���߳�ID��" << thid << endl;
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