//
// Created by autocars on 2023/5/15.
//

#include "ThreadPool.h"

void ThreadPool::addTask(ThreadPool::Task &&task) {
    m_queue.put(forward<Task>(task));
}

void ThreadPool::addTask(const ThreadPool::Task &task) {
    m_queue.put(task);
}

void ThreadPool::Start(int numThreads) {
    m_running = true;
    for (int i = 0; i < numThreads; ++i) {
        m_threadgroup.push_back(make_shared<thread>(&ThreadPool::runInThread, this));
    }
}

void ThreadPool::Stop() {
    call_once(m_flag, [this]{stopThreadGroup();});
}

void ThreadPool::runInThread() {
    while (m_running){
        list<Task> list;
        m_queue.take(list);
        for(auto &task : list){
            if(!m_running){
                return;
            }
            task();
        }
    }
}

void ThreadPool::stopThreadGroup() {
    m_queue.stop();
    m_running = false;
    for(auto thread : m_threadgroup){
        if(thread) thread->join();
    }
    m_threadgroup.clear();
}
