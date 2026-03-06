//
// Created by fisp on 23.02.2026.
//


#ifndef HOMEWORK_FUNCTIONS_H
#define HOMEWORK_FUNCTIONS_H

#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <thread>

enum class Status {
    Good,
    Error,
    Warning,
    None
};

struct ResultStatus {
    Status condition = Status::None;
    std::string message;
    int line = 0;
    const char* file = "";

    static ResultStatus Error(const std::string &msg);
    static ResultStatus Warning(const std::string &msg);
    static ResultStatus Good();

    bool isWarning() const;
    bool isError() const;
    bool isGood() const;
    bool isNone() const;
};

void processInputType(std::string &type);
ResultStatus processInputName(std::string &name);

ResultStatus processInputVector(const std::string &type,
                      std::vector<std::string> &stringVec,
                      std::vector<int> &intVec,
                      std::vector<float> &floatVec, uint64_t size);

bool isIpAddress(const std::string &ip);
bool isPort(const std::string &port);
void logger(const ResultStatus &result);

std::string fixInputString(const std::string &str);
std::string stringStrip(const std::string &str);
bool hexStringIsIp(const std::string &str);
uint32_t hexStringToUint32_t(const std::string &str);

ResultStatus fillVectors(std::vector<std::string> &stringVec,
    std::vector<int> &intVec,
    std::vector<float> &floatVec,
    const std::string &type,
    const std::string &str,
    uint64_t vecSize);

ResultStatus fillIntVector(const std::string &str,
    std::vector<int> &intVec, uint64_t vecSize);

ResultStatus fillFloatVector(const std::string &str,
    std::vector<float> &floatVec, uint64_t vecSize);

ResultStatus fillStringVector(const std::string &str,
    std::vector<std::string> &stringVec, uint64_t vecSize);

std::vector<std::string> split(const std::string &str, char separator);

template <typename T>
void printVector(const std::vector<T> &vec, const int &size, const std::string &type) {
    std::cout << type << ": ";
    for (int i = 0; i < size; i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

constexpr inline uint64_t hashString(const char* str) {
    uint64_t hash = 0;
    while (*str) {
        hash = hash * 31 + static_cast<uint64_t>(*str);
        str++;
    }
    return hash;
}

#endif //HOMEWORK_FUNCTIONS_H