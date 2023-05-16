cmake_minimum_required(VERSION 3.21)
project(ThreadPool_Cpp11)

set(CMAKE_CXX_STANDARD 14)

add_executable(ThreadPool_Cpp11 Syncqueue.h ThreadPool.cpp ThreadPool.h test.cpp)
