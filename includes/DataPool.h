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
    std::vector<int> intVec;
    std::vector<float> floatVec;
};

// Инвариант - countElem всегда в диапазоне [0, N]
// head и tail всегда в диапазоне [0, N-1]
// v_arr указывает на массив размером N
class DataPool {
    int N = 0;
    MyVec *v_arr = new MyVec[N];
    int tail = 0;
    int head = 0;
    int countElem = 0;

    public:
    explicit DataPool(const int size) : N(size) {};
    ~DataPool();

    bool insert(const MyVec &vec);
    MyVec first();
    MyVec last() const;
    bool empty() const;
};

#endif //HOMEWORK_DATAPOOL_H