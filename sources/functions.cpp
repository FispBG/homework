//
// Created by fisp on 23.02.2026.
//
#include "../includes/functions.h"

#include <iomanip>
#include <chrono>
#include <fstream>

ResultStatus ResultStatus::Good() {
    ResultStatus good = {Status::Good, "", 0, ""};
    return good;
}

ResultStatus ResultStatus::Error(const std::string &msg) {
    ResultStatus err = {Status::Error, msg, __LINE__, __FILE__};
    return err;
}

ResultStatus ResultStatus::Warning(const std::string &msg) {
    ResultStatus warning = {Status::Warning, msg, __LINE__, __FILE__};
    return warning;
}

bool ResultStatus::isError() const {
    return condition == Status::Error;
}

bool ResultStatus::isWarning() const {
    return condition == Status::Warning;
}

bool ResultStatus::isGood() const {
    return condition == Status::Good;
}

bool ResultStatus::isNone() const {
    return condition == Status::None;
}

void logger(const ResultStatus &result) {
    if (result.isGood()) {
        return;
    }
    const std::string typeLog = result.isError() ? "[Error]" : "[Warning]";

    const auto systemTime = std::chrono::system_clock::now();
    const auto normalTime = std::chrono::system_clock::to_time_t(systemTime);

    const auto threadId = std::this_thread::get_id();

    std::ostringstream logString;

    logString << std::put_time(std::localtime(&normalTime), "%Y-%m-%d %H:%M:%S")
    << " | " << typeLog << " | " << "Thread: " << threadId << " | "
    << result.message << " | " << result.file << " | " << "line: " << result.line << std::endl;

    std::ofstream logFile("log.txt", std::ios_base::app);
    if (logFile.is_open()) {
        logFile << logString.str();
        logFile.close();
    }

    if (result.isError()) {
        std::cerr << result.message << std::endl;
    }else {
        std::cout << result.message << std::endl;
    }
}

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

ResultStatus fillVectors(std::vector<std::string> &stringVec,
    std::vector<int64_t> &intVec,
    std::vector<float> &floatVec,
    const std::string &type,
    const std::string &str,
    const uint64_t vecSize) {

    const uint64_t hashType = hashString(type.c_str());

    switch (hashType) {
        case hashString("int"):
            return fillVector(str, intVec, vecSize);

        case hashString("float"):
            return fillVector(str, floatVec, vecSize);

        case hashString("string"):
            return fillVector(str, stringVec, vecSize);

        default:
            return ResultStatus::Error("Unknown hash type.");
    }
}

void processInputType(std::string &type)  {

    std::string inputStr;
    std::cout << "Input type(int, float, string): ";
    std::getline(std::cin, inputStr);

    inputStr = fixInputString(inputStr);
    if (inputStr != "string" && inputStr != "int" && inputStr != "float") {
        logger(ResultStatus::Error("Invalid type: " + inputStr));
        std::cout << "Press enter: ";
        getchar();
        return;
    }

    type = inputStr;
}

ResultStatus processInputVector(const std::string &type,
                      std::vector<std::string> &stringVec,
                      std::vector<int64_t> &intVec,
                      std::vector<float> &floatVec, const uint64_t size)  {

    std::string inputStr;
    std::cout << "Input " << type << " vector: ";
    std::getline(std::cin, inputStr);
    const ResultStatus res = fillVectors(stringVec, intVec, floatVec, type, inputStr, size);
    logger(res);

    if (res.isError()) {
        std::cout << "Press enter: ";
        getchar();
    }

    return res;
}

ResultStatus processInputName(std::string &name) {

    std::cout << "Input name: ";
    std::string inputStr;
    std::getline(std::cin, inputStr);
    if (inputStr.empty()) {
        ResultStatus res = ResultStatus::Error("Input empty name.");
        logger(res);
        std::cout << "Press enter: ";
        getchar();
        return res;
    }

    name = std::move(inputStr);
    return ResultStatus::Good();
}

std::vector<std::string> split(const std::string &str, const char separator) {
    std::vector<std::string> stringVec;

    std::stringstream ss(str);
    std::string subStr;

    while (getline(ss, subStr, separator)) {
        stringVec.push_back(subStr);
    }

    return stringVec;
}
