//
// Created by fisp on 23.02.2026.
//

#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#ifndef HOMEWORK_FUNCTIONS_H
#define HOMEWORK_FUNCTIONS_H

struct Config{
    std::string address;
    int32_t port = 0;
    std::string role;
    int32_t id = 0;
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

void processingFlag(uint64_t flag, Config &config, const std::string& argFlag);
Config createConfig(const int &argc, const char *argv[]);

std::string lowerString(std::string str);

bool fillVectors(std::vector<std::string> &stringVec,
    std::vector<int> &intVec,
    std::vector<float> &floatVec,
    const std::string &type,
    const std::string &str,
    int vecSize);

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