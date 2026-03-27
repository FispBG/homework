//
// Created by fisp on 27.03.2026.
//

#include "../includes/StringFunction.h"
#include <sstream>

std::vector<std::string> split(const std::string &str, const char separator) {
    std::vector<std::string> stringVec;

    std::stringstream ss(str);
    std::string subStr;

    while (getline(ss, subStr, separator)) {
        stringVec.push_back(subStr);
    }

    return stringVec;
}

std::string stringStrip(const std::string &str) {
    const uint64_t startText = str.find_first_not_of(" \t\n\r\f\v");

    if (startText == std::string::npos) {
        return "";
    }
    const uint64_t endText = str.find_last_not_of(" \t\n\r\f\v");

    return str.substr(startText, endText - startText + 1);
}

std::string fixInputString(const std::string &str) {
    std::string newString;

    for (const char ch : str) {
        const uint8_t chValue = std::tolower(ch);

        if (std::isprint(chValue)) {
            newString.push_back(static_cast<char>(chValue));
        }
    }

    return stringStrip(newString);
}

bool hexStringIsIpWithPort(const std::string &str) {
    if (str.length() != 14) {
        return false;
    }

    if (str[0] != '0' || (str[1] != 'x' && str[1] != 'X')) {
        return false;
    }

    for (uint8_t i = 2; i < str.length(); i++) {
        char ch = str[i];

        if (!((ch >= '0' && ch <= '9') ||
            (ch >= 'a' && ch <= 'f') ||
            (ch >= 'A' && ch <= 'F')
            )) {
            return false;
            }
    }

    return true;
}

uint64_t hexStringToUint64_t(const std::string &str) {
    uint64_t hexInt;
    std::stringstream ss;

    ss << std::hex << str;
    ss >> hexInt;

    return hexInt;
}