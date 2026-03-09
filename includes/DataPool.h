//
// Created by fisp on 28.02.2026.
//

#ifndef HOMEWORK_DATAPOOL_H
#define HOMEWORK_DATAPOOL_H

#include "../includes/functions.h"

// Инвариант - type является одним из: int, float, string
struct MyVec {
    std::string type;
    std::vector<std::string> stringVec;
    std::vector<int64_t> intVec;
    std::vector<float> floatVec;
};

// Инвариант - countElem всегда в диапазоне [0, N]
// head и tail всегда в диапазоне [0, N-1]
// v_arr указывает на массив размером N
template <typename T, uint64_t N>
class DataPool {
    std::unique_ptr<T[]> v_arr = std::make_unique<T[]>(N);
    int tail = 0;
    int head = 0;
    int countElem = 0;

    public:
    explicit DataPool() = default;
    ~DataPool() = default;

    bool insert(const T &vec);
    T first();
    T last() const;
    bool empty() const;
};

template <typename T, uint64_t N>
bool DataPool<T, N>::insert(const T &vec) {
    if (countElem < N) {
        v_arr[tail] = vec;
        countElem++;
        tail = (tail + 1) % N;
        return true;
    }
    logger(ResultStatus::Warning("Vector pool is full."));
    return false;
}
template <typename T, uint64_t N>
T DataPool<T, N>::first() {
    if (countElem > 0) {
        MyVec vec = v_arr[head];
        countElem--;
        head = (head + 1) % N;
        return vec;
    }

    logger(ResultStatus::Warning("Vector pool is empty."));
    return T();
}

template <typename T, uint64_t N>
T DataPool<T, N>::last() const {
    if (countElem > 0) {
        MyVec vec = v_arr[(tail - 1 + N) % N];
        return vec;
    }
    logger(ResultStatus::Warning("Vector pool is empty."));
    return T();
}

template <typename T, uint64_t N>
bool DataPool<T, N>::empty() const {
    if (countElem > 0) {
        return false;
    }

    return true;
}

#endif //HOMEWORK_DATAPOOL_H