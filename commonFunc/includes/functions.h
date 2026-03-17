//
// Created by fisp on 23.02.2026.
//

#pragma once

#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <sstream>
#include "./StandardPackets.h"
#include <arpa/inet.h>

enum class Status {
    Good,
    Error,
    Warning,
    None
};

struct ResultStatus {
    Status condition = Status::None;
    std::string message;
    int64_t line = 0;
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
                      std::vector<int64_t> &intVec,
                      std::vector<float> &floatVec, uint64_t size);

bool isIpAddress(const std::string &ip);
bool isPort(const std::string &port);
void logger(const ResultStatus &result);

std::string fixInputString(const std::string &str);
std::string stringStrip(const std::string &str);
bool hexStringIsIpWithPort(const std::string &str);
uint64_t hexStringToUint64_t(const std::string &str);

ResultStatus fillVectors(std::vector<std::string> &stringVec,
    std::vector<int64_t> &intVec,
    std::vector<float> &floatVec,
    const std::string &type,
    const std::string &str,
    uint64_t vecSize);

std::vector<std::string> split(const std::string &str, char separator);

PacketString createPacketString(const std::string &str);

template<typename T>
ResultStatus fillVector(const std::string &str,
    std::vector<T> &vecInput, const uint64_t vecSize) {

    std::istringstream ss(str);
    std::string elementString;
    std::vector<T> vecTemp;
    uint64_t count = 0;

    while (std::getline(ss, elementString, ' ')) {
        if (count == vecSize) {
            logger(ResultStatus::Warning("Size more, then vecSize."));
            break;
        }

        T elementTryConvert;
        std::istringstream convertStream(elementString);

        if (!(convertStream >> elementTryConvert) || !convertStream.eof()) {
            return ResultStatus::Error("Invalid element: " + elementString);
        }

        vecTemp.push_back(elementTryConvert);
        count++;
    }

    if (count < vecSize) {
        return ResultStatus::Error("Need more argument input.");
    }
    if constexpr (std::is_arithmetic_v<T>) {
        if (vecTemp[3] == 0) {
            return ResultStatus::Error("W-component invalid: 0.");
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

constexpr inline uint64_t hashString(const char* str) {
    uint64_t hash = 0;
    while (*str) {
        hash = hash * 31 + static_cast<int>(*str);
        str++;
    }
    return hash;
}

template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
PacketVector createPacketVector(T x, T y, T z, T w) {
    PacketVector packet{};

    auto toNetwork = [](T value) {
        uint32_t bytes;
        std::memcpy(&bytes, &value, sizeof(uint32_t));
        return ntohl(bytes);
    };

    packet.data.x = toNetwork(x);
    packet.data.y = toNetwork(y);
    packet.data.z = toNetwork(z);
    packet.data.w = toNetwork(w);

    return packet;
}

template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
PacketVector createPacketVector(T x, T y, T z, T w) {
    return createPacketVector(
        static_cast<float>(x),
        static_cast<float>(y),
        static_cast<float>(z),
        static_cast<float>(w)
    );
}
