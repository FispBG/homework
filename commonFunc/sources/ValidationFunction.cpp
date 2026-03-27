//
// Created by fisp on 27.03.2026.
//

#include "../includes/ValidationFunction.h"
#include "../includes/StringFunction.h"
#include <sstream>

bool isIpAddress(const std::string &ip) {
    std::stringstream ss(ip);
    std::string byte;
    uint64_t count = 0;

    if (ip.back() == '.') {
        return false;
    }

    while (std::getline(ss, byte, '.')) {

        if (!std::isdigit(static_cast<unsigned char>(byte.front()))) {
            return false;
        }

        if (byte.length() > 1 && byte.front() == '0') {
            return false;
        }

        char* firstNotInt = nullptr;
        const int64_t num = strtol(byte.c_str(), &firstNotInt, 10);

        if (*firstNotInt != '\0') {
            return false;
        }

        if (num < 0 || num > 255) {
            return false;
        }
        count++;
    }

    return count == 4;
}

bool isPort(const std::string &port) {
    if (port.empty()) {
        return false;
    }

    char* firstNotInt = nullptr;
    const int64_t portValue = strtol(port.c_str(), &firstNotInt, 10);

    if (*firstNotInt != '\0') {
        return false;
    }

    if (portValue <= 65535 && portValue >= 0) {
        return true;
    }
    return false;
}

void processInputType(std::string &type)  {

    std::string inputStr;
    std::cout << "Input type(int, float, string): ";
    std::getline(std::cin, inputStr);

    inputStr = fixInputString(inputStr);
    if (inputStr != "string" && inputStr != "int" && inputStr != "float") {
        logger(RES_ERROR("Invalid type: " + inputStr));
        std::cout << "Press enter: ";
        getchar();
        return;
    }

    type = inputStr;
}

ResultStatus processInputName(std::string &name) {

    std::cout << "Input name: ";
    std::string inputStr;
    std::getline(std::cin, inputStr);
    if (inputStr.empty()) {
        ResultStatus res = RES_ERROR("Input empty name.");
        logger(res);
        std::cout << "Press enter: ";
        getchar();
        return res;
    }

    name = std::move(inputStr);
    return ResultStatus::Good();
}