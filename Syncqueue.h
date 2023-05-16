//
// Created by autocars on 2023/5/15.
//

#ifndef THREADPOOL_CPP11_SYNCQUEUE_H
#define THREADPOOL_CPP11_SYNCQUEUE_H

#include <list>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>
using namespace std;

template<class T>
class Syncqueue {
public:
    Syncqueue(int maxSize);

    ~Syncqueue();

    Syncqueue<T> operator=(Syncqueue<T>&& syncqueue);
    void put(const T &value);

    void put(T &&value);

    void take(list<T> &list);

    void take(T &value);

    void stop();

    bool empty();

    bool full();

    size_t size();

    int count();

private:
    bool notFull() const;
    bool notEmpty() const;

    template<class F>
    void add(F&& x);

private:
    list<T> m_queue;
    mutex m_mutex;
    condition_variable m_notEmpty;
    condition_variable m_notFUll;
    int m_maxSize;
    bool m_needStop;
};

template<class T>
Syncqueue<T>::Syncqueue(int maxSize): m_maxSize(maxSize), m_needStop(false) {

}

template<class T>
Syncqueue<T>::~Syncqueue() = default;

template<class T>
void Syncqueue<T>::put(const T &value) {
    add(value);

}

template<class T>
void Syncqueue<T>::put(T &&value) {
    add(std::forward<T>(value));
}

template<class T>
void Syncqueue<T>::take(list<T> &list) {
    unique_lock<mutex> locker(m_mutex);
    m_notEmpty.wait(locker, [this](){return this->m_needStop || this->notEmpty();});
    if(m_needStop) return;
    list = std::move(m_queue);
    m_notFUll.notify_one();
}

template<class T>
void Syncqueue<T>::take(T &value) {
    unique_lock<mutex> locker(m_mutex);
    m_notEmpty.wait(locker, [this](){return this->m_needStop || this->notEmpty();});
    if(m_needStop) return;
    value = m_queue.front();
    m_queue.pop_front();
    m_notFUll.notify_one();
}

template<class T>
void Syncqueue<T>::stop() {
    {
        lock_guard<mutex> locker(m_mutex);
        m_needStop = true;
    }
    m_notFUll.notify_all();
    m_notEmpty.notify_all();
}

template<class T>
bool Syncqueue<T>::empty() {
    lock_guard<mutex> locker(m_mutex);
    return m_queue.empty();
}

template<class T>
bool Syncqueue<T>::full() {
    lock_guard<mutex> locker(m_mutex);
    return m_queue.size() == m_maxSize;
}

template<class T>
size_t Syncqueue<T>::size() {
    lock_guard<mutex> locker(m_mutex);
    return m_queue.size();
}

template<class T>
int Syncqueue<T>::count() {
    return m_queue.size();
}

template<class T>
bool Syncqueue<T>::notFull() const {
    bool full = m_queue.size() >= m_maxSize;
    if(full) cout << "缓冲区满了，需要等待..." << endl;
    return !full;
}

template<class T>
bool Syncqueue<T>::notEmpty() const {
    bool empty = m_queue.size() == 0;
    if(empty) cout << "缓冲区空了，需要等待..., 异步层线程的ID:" << this_thread::get_id() <<  endl;
    return !empty;
}

template<class T>
template<class F>
void Syncqueue<T>::add(F &&x) {
    unique_lock<mutex> locker(m_mutex);
    m_notFUll.wait(locker, [this](){return this->m_needStop || this->notFull();});
    if (m_needStop) return;
    m_queue.push_back(std::forward<F>(x));
    m_notEmpty.notify_one();

}

template<class T>
Syncqueue<T> Syncqueue<T>::operator=(Syncqueue<T> &&syncqueue) {
    return syncqueue;
}

#endif //THREADPOOL_CPP11_SYNCQUEUE_H
