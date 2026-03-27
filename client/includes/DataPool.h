//
// Created by fisp on 28.02.2026.
//

#pragma once

#include <vector>
#include <array>
#include "../../commonFunc/includes/ResultStatus.h"

// Инвариант - type является одним из: int, float, string
class MyVec {
    std::string type;
    std::vector<std::string> stringVec;
    std::vector<int64_t> intVec;
    std::vector<float> floatVec;

    public:
    MyVec() : type("") {};
    explicit MyVec(const std::vector<std::string> &vec) : type("string"), stringVec(std::move(vec)) {};
    explicit MyVec(const std::vector<std::int64_t> &vec) : type("int"), intVec(std::move(vec)) {};
    explicit MyVec(const std::vector<float> &vec) : type("float"), floatVec(std::move(vec)) {};

    std::string getType() const;

    std::vector<std::string>& getStringVec();
    std::vector<int64_t>& getIntVec();
    std::vector<float>& getFloatVec();
};

// Инвариант - countElem всегда в диапазоне [0, N]
// head и tail всегда в диапазоне [0, N-1]
// v_arr указывает на массив размером N
template <typename T, uint64_t N>
class DataPool {
    std::array<T , N> v_arr;
    size_t tail = 0;
    size_t head = 0;
    size_t countElem = 0;

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
