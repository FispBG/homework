//
// Created by fisp on 01.03.2026.
//
#include "../includes/DataPool.h"
#include "../includes/functions.h"

DataPool::~DataPool() {
    delete[] v_arr;
}

bool DataPool::insert(const MyVec &vec) {
    if (countElem < N) {
        v_arr[tail] = vec;
        countElem++;
        tail = (tail + 1) % N;
        return true;
    }
    logger(ResultStatus::Warning("Vector pool is full."));
    return false;
}

MyVec DataPool::first() {
    if (countElem > 0) {
        MyVec vec = v_arr[head];
        countElem--;
        head = (head + 1) % N;
        return vec;
    }

    logger(ResultStatus::Warning("Vector pool is empty."));
    return MyVec();
}

MyVec DataPool::last() const {
    if (countElem > 0) {
        MyVec vec = v_arr[(tail - 1 + N) % N];
        return vec;
    }
    logger(ResultStatus::Warning("Vector pool is empty."));
    return MyVec();
}

bool DataPool::empty() const {
    if (countElem > 0) {
        return false;
    }

    return true;
}
