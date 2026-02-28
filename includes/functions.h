//
// Created by fisp on 23.02.2026.
//

#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

#ifndef HOMEWORK_FUNCTIONS_H
#define HOMEWORK_FUNCTIONS_H
enum Status {
    Good_Stat,
    Error_Stat,
    Warning_Stat,
};

struct ResultStatus {
    Status condition;
    std::string message;
    int line = 0;
    const char* file = "";

    static ResultStatus Error(const std::string &msg);
    static ResultStatus Warning(const std::string &msg);
    static ResultStatus Good();

    bool isWarning() const;
    bool isError() const;
    bool isGood() const;
};

struct Config{
    std::string address;
    int32_t port = -1;
    std::string role = "Client";
    int32_t id = -1;
    std::string lib;
};
void processInputType(std::string &type);
void processInputName(std::string &name);
bool commandForUser(uint64_t hashCommand, std::string &name);
bool commandForVector(const uint64_t &hashCommand, std::string &type,
                    std::vector<std::string> &stringVec,
                    std::vector<int> &intVec, std::vector<float> &floatVec,
                    int size);
void processInputVector(const std::string &type,
                      std::vector<std::string> &stringVec,
                      std::vector<int> &intVec,
                      std::vector<float> &floatVec, int size);

bool isIpAddress(const std::string &ip);
bool isPort(const std::string &port);

ResultStatus processingFlag(uint64_t hashFlag, const std::string &flag,
    Config &config, const std::string& argFlag);
ResultStatus createConfig(const int &argc, const char *argv[], Config &config);

std::string fixInputString(const std::string &str);

ResultStatus fillVectors(std::vector<std::string> &stringVec,
    std::vector<int> &intVec,
    std::vector<float> &floatVec,
    const std::string &type,
    const std::string &str,
    int vecSize);

ResultStatus fillIntVector(const std::string &str,
    std::vector<int> &intVec, int vecSize);

ResultStatus fillFloatVector(const std::string &str,
    std::vector<float> &floatVec, int vecSize);

ResultStatus fillStringVector(const std::string &str,
    std::vector<std::string> &stringVec, int vecSize);

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