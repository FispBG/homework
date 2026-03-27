//
// Created by fisp on 27.03.2026.
//

#pragma once

#include <vector>
#include <string>
#include <stdint.h>

std::vector<std::string> split(const std::string &str, char separator);
std::string fixInputString(const std::string &str);
std::string stringStrip(const std::string &str);
bool hexStringIsIpWithPort(const std::string &str);
uint64_t hexStringToUint64_t(const std::string &str);

constexpr inline uint64_t hashString(const char* str) {
    uint64_t hash = 0;
    while (*str) {
        hash = hash * 31 + static_cast<int>(*str);
        str++;
    }
    return hash;
}