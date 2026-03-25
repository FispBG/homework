//
// Created by fisp on 28.02.2026.
//

#pragma once

#include "../../commonFunc/includes/functions.h"

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
    std::array<T , N> v_arr;
    uint32_t tail = 0;
    uint32_t head = 0;
    uint32_t countElem = 0;

    public:
    DataPool() = default;
    ~DataPool() = default;

    bool insert(const T &vec);
    T first() const;
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
    logger(RES_WARNING("Vector pool is full."));
    return false;
}
template <typename T, uint64_t N>
T DataPool<T, N>::first() const{
    if (countElem > 0) {
        T vec = v_arr[head];
        return vec;
    }

    logger(RES_WARNING("Vector pool is empty."));
    return T();
}

template <typename T, uint64_t N>
T DataPool<T, N>::last() const {
    if (countElem > 0) {
        T vec = v_arr[(tail - 1 + N) % N];
        return vec;
    }
    logger(RES_WARNING("Vector pool is empty."));
    return T();
}

template <typename T, uint64_t N>
bool DataPool<T, N>::empty() const {
    if (countElem > 0) {
        return false;
    }

    return true;
}
