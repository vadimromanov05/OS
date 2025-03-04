#include "../include/ThreadPool.h"
#include <stdexcept>

ThreadPool::ThreadPool(size_t threads) : stop(false) {
    //std::cout << "thread_start " << threads << "\n";
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty();});
                    if (this->stop && this->tasks.empty()) return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
        });
    }
    //std::cout << "thread_end\n";
}

ThreadPool::~ThreadPool() {
    //std::cout << "thread_pool_start\n";
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers) {
        worker.join();
    }
    //std::cout << "thread_pool_end\n";
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");
        tasks.emplace(task);
    }
    condition.notify_one();
    //std::cout << "thread_enqueue\n";
}