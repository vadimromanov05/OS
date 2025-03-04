#include "../include/Barrier.h"

Barrier::Barrier(size_t count)
    : threshold(count), count(count), generation(0) {}

void Barrier::wait() {
    //std::cout << "barrier_start\n";
    std::unique_lock<std::mutex> lock(mtx);
    //std::cout << "Mutex is locked" << '\n';
    auto gen = generation;
    //std::cout << "Generation: " << generation << '\n';
    //std::cout << "Count: " << count << '\n';
    if (--count == 0) {
        //std::cout << "Barrier_if" << '\n';
        generation++;
        count = threshold;
        cv.notify_all();
    } else {
        //std::cout << "Barrier_else " << "Gen: " << gen << '\n';
        cv.wait(lock, [this, gen] { return gen != generation; });
    }
    //std::cout << "barrier_end\n";
}