//
// Created by fisp on 27.03.2026.
//

#pragma once

#include <cstring>

#include "./ResultStatus.h"
#include <sstream>
#include "./StringFunction.h"

ResultStatus processInputVector(const std::string &type,
                                std::vector<std::string> &stringVec,
                                std::vector<int64_t> &intVec,
                                std::vector<float> &floatVec, uint64_t size);

ResultStatus fillVectors(std::vector<std::string> &stringVec,
    std::vector<int64_t> &intVec,
    std::vector<float> &floatVec,
    const std::string &type,
    const std::string &str,
    uint64_t vecSize);


template<typename T>
ResultStatus fillVector(const std::string &str,
    std::vector<T> &vecInput, const uint64_t vecSize) {

    std::istringstream ss(str);
    std::string elementString;
    std::vector<T> vecTemp;
    uint64_t count = 0;

    while (std::getline(ss, elementString, ' ')) {
        if (count == vecSize) {
            logger(RES_WARNING("Size more, then vecSize."));
            break;
        }

        T elementTryConvert;
        std::istringstream convertStream(elementString);

        if (!(convertStream >> elementTryConvert) || !convertStream.eof()) {
            return RES_ERROR("Invalid element: " + elementString);
        }

        vecTemp.push_back(elementTryConvert);
        count++;
    }

    if (count < vecSize) {
        return RES_ERROR("Need more argument input.");
    }
    if constexpr (std::is_arithmetic_v<T>) {
        if (vecTemp[3] == 0) {
            return RES_ERROR("W-component invalid: 0.");
        }
    }

    vecInput = std::move(vecTemp);
    return ResultStatus::Good();
}

template <typename T>
void printVector(const std::vector<T> &vec, const uint64_t &size, const std::string &type) {
    std::cout << type << ": ";
    for (uint64_t i = 0; i < size && i < vec.size(); i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

template <typename processFunc, typename helpFunc>
ResultStatus loopForInputArgs(const int argc, const char *argv[], processFunc process, helpFunc help) {
    for (int i = 1; i < argc; i++) {
        const uint64_t hashFlag = hashString(argv[i]);
        const std::string flag = argv[i];

        if (strcmp(argv[i],"--help") == 0) {
            help();
            exit(0);
        }

        if (i+1 >= argc) {
            return RES_ERROR("No argument for flag: " + flag);
        }

        std::string argFlag = argv[++i];

        if (argFlag[0] == '-') {
            return RES_ERROR("Invalid arg flag: " + argFlag);
        }

        const ResultStatus res = process(hashFlag, flag, argFlag);
        logger(res);

        if (res.isError()) {
            return res;
        }
    }

    return ResultStatus::Good();
}
