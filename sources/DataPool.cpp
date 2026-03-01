//
// Created by fisp on 01.03.2026.
//
#include "../includes/DataPool.h"


DataPool::~DataPool() {
    delete[] v_arr;
}

void DataPool::insert(const MyVec &vec) {
    if (countElem < N) {
        v_arr[tail] = vec;
        countElem++;
        tail = (tail + 1) % N;
    }else {
        logger(ResultStatus::Warning("Vector pool is full."));
    }
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