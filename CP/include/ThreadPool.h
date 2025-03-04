#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <iostream>

class ThreadPool {
public:
    ThreadPool(size_t threads);
    ~ThreadPool();
    void enqueue(std::function<void()> task);
    std::condition_variable condition;
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    bool stop;
};

#endif