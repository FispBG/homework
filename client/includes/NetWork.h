//
// Created by fisp on 04.03.2026.
//

#pragma once

#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <cstdint>

struct ResultStatus;

// Инвариант - байты ip в диапазоне [0, 255]
// port в диапазоне [0, 65535]
// ipInput == true адрес установлен
class IpAddress {
    std::array<uint16_t, 4> ip = {0, 0, 0, 0};
    uint16_t port = 0;
    bool ipInput = false;
public:
    explicit IpAddress() = default;

    ResultStatus setIpAddress(uint64_t hex);
    ResultStatus setIpAddress(const std::string &ipStr);
    ResultStatus setIpAddress(const std::vector<int64_t> &ipVectorInt);
    std::string getAddress() const;
    bool checkIpInput() const;

    uint16_t getPort() const;
    std::string getIp() const;
    friend std::ostream& operator << (std::ostream& os, const IpAddress& netAddress);
};
