//
// Created by fisp on 04.03.2026.
//

#ifndef HOMEWORK_NETWORK_H
#define HOMEWORK_NETWORK_H
#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <cstdint>

struct ResultStatus;

class IpAddress {
    std::array<uint32_t, 4> ip = {0, 0, 0, 0};
    bool ipInput = false;
public:
    explicit IpAddress() = default;

    ResultStatus setIpAddress(uint32_t hex);
    ResultStatus setIpAddress(const std::string &ipStr);
    ResultStatus setIpAddress(const std::vector<int32_t> &ipVectorInt);

    bool getIpAddress() const;

    friend std::ostream& operator << (std::ostream& os, const IpAddress& netAddress);
};

#endif //HOMEWORK_NETWORK_H