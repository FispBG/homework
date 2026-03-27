//
// Created by fisp on 25.03.2026.
//

#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <cstdint>
#include <vector>
#include <functional>
#include <queue>

class ThreadPool {
    std::mutex mutex;
    std::vector<std::thread> threads;
    std::atomic<bool> stop = false;
    std::queue<std::function<void()>> tasks;
    std::condition_variable condition;

    void runThread();
    public:
    explicit ThreadPool(uint32_t countUser);
    ~ThreadPool();
    void addNewTask(const std::function<void()> &func);

};
