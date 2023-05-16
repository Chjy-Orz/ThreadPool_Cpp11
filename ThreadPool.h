//
// Created by autocars on 2023/5/15.
//

#ifndef THREADPOOL_CPP11_THREADPOOL_H
#define THREADPOOL_CPP11_THREADPOOL_H

#include <list>
#include <thread>
#include <functional>
#include <memory>
#include <atomic>
#include "Syncqueue.h"

using namespace std;
const int MaxTaskCount = 100;
class ThreadPool {
public:
    using Task = function<void()>;
    ThreadPool(int numThreads = thread::hardware_concurrency()): m_queue(MaxTaskCount){
        Start(numThreads);
    }
    ~ThreadPool(){
        Stop();
    }
    void Start(int numThreads);
    void Stop();
    void addTask(Task&& task);
    void addTask(const Task& task);

private:
    void runInThread();
    void stopThreadGroup();

private:
    list<shared_ptr<thread>> m_threadgroup;
    Syncqueue<Task> m_queue;
    atomic_bool m_running;
    once_flag m_flag;
};


#endif //THREADPOOL_CPP11_THREADPOOL_H
