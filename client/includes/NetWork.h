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
    uint16_t port = -1;
    bool ipInput = false;
    bool portInput = false;

public:
    explicit IpAddress() = default;

    ResultStatus setAddress(uint64_t hex);

    bool setPort(const std::string &inputPort);
    bool setIp(const std::string &inputIp);

    ResultStatus setAddress(const std::string &ipStr);
    ResultStatus setAddress(const std::vector<int64_t> &ipVectorInt);
    std::string getAddress() const;
    bool checkIpInput() const;

    bool checkPortInput() const;

    uint16_t getPort() const;
    std::string getIp() const;
    friend std::ostream& operator << (std::ostream& os, const IpAddress& netAddress);
};
