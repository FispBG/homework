//
// Created by fisp on 25.03.2026.
//

#include "../includes/ThreadPool.h"

ThreadPool::ThreadPool(const uint32_t countUser) {
    for (uint32_t i = 0; i < countUser; i++) {
        threads.push_back(std::thread(&ThreadPool::runThread, this));
    }
};

void ThreadPool::addNewTask(const std::function<void()> &func) {
    {
        std::lock_guard<std::mutex> lock(mutex);
        tasks.push(func);
    }
    condition.notify_one();
}

void ThreadPool::runThread() {
    while (true) {
        std::function<void()> func;
        {
            std::unique_lock<std::mutex> lock(mutex);
            while (!stop && tasks.empty()) {
                condition.wait(lock);
            }
            if (tasks.empty() && stop) {
                return;
            }
            func = std::move(tasks.front());
            tasks.pop();
        }
        func();
    }
}

ThreadPool::~ThreadPool() {
    stop = true;
    condition.notify_all();
    for (std::thread &thread : threads) {
        thread.join();
    }
}
