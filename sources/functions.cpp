//
// Created by fisp on 23.02.2026.
//
#include "../includes/functions.h"

#include <fstream>
#include <iomanip>

ResultStatus ResultStatus::Good() {
    ResultStatus good = {Good_Stat, "", 0, ""};
    return good;
}

ResultStatus ResultStatus::Error(const std::string &msg) {
    ResultStatus err = {Error_Stat, msg, __LINE__, __FILE__};
    return err;
}

ResultStatus ResultStatus::Warning(const std::string &msg) {
    ResultStatus warning = {Warning_Stat, msg, __LINE__, __FILE__};
    return warning;
}

bool ResultStatus::isError() const {
    return condition == Error_Stat;
}

bool ResultStatus::isWarning() const {
    return condition == Warning_Stat;
}

bool ResultStatus::isGood() const {
    return condition == Good_Stat;
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
    int count = 0;

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

std::string fixInputString(const std::string &str) {
    std::string newString;

    for (const char ch : str) {
        const int chValue = std::tolower(ch);
        if (std::isprint(chValue)) {
            newString.push_back(static_cast<char>(chValue));
        }
    }

    return newString;
}

template<typename T, typename Transform>
ResultStatus validateNumberString(const std::string &str, std::vector<T> &vecNumbers,
    const int vecSize, Transform transform) {

    std::istringstream ss(str);
    std::string element;
    int count = 0;

    while (getline(ss, element, ' ')) {
        if (count == vecSize) {
            return ResultStatus::Warning("Size more, then vecSize.");
        }

        char* firstNotInt = nullptr;
        vecNumbers[count] = transform(element.c_str(), &firstNotInt);
        if (*firstNotInt != '\0') {
            return ResultStatus::Error("Invalid number: " + element);
        }

        count++;
    }

    if (count < vecSize) {
        return ResultStatus::Error("Need more argument input.");
    }
    if (count == vecSize && vecNumbers[3] == 0) {
        return ResultStatus::Error("W-component invalid: " + element);
    }

    return ResultStatus::Good();
}

ResultStatus validateString(const std::string &str, const int vecSize,
    std::vector<std::string> &vecString) {

    std::istringstream ss(str);
    std::string element;
    int count = 0;

    while (getline(ss, element, ' ')) {
        if (count == vecSize) {
            return ResultStatus::Warning("Size more, then vecSize.");
        }

        vecString[count] = element;
        count++;
    }

    if (count < vecSize) {
        return ResultStatus::Error("Need more argument input.");
    }

    return ResultStatus::Good();
}

long stringToLongDec(const char* str, char** end) {
    return std::strtol(str, end, 10);
}

ResultStatus fillIntVector(const std::string &str,
    std::vector<int> &intVec, const int vecSize) {

    std::vector<int> tempInt(vecSize);
    ResultStatus res = validateNumberString(str, tempInt, vecSize, stringToLongDec);
    if (!res.isError()) {
        intVec = std::move(tempInt);
    }

    return res;
}

ResultStatus fillFloatVector(const std::string &str,
    std::vector<float> &floatVec, const int vecSize) {

    std::vector<float> tempFloat(vecSize);
    ResultStatus res = validateNumberString(str, tempFloat, vecSize, strtod);
    if (!res.isError()) {
        floatVec = std::move(tempFloat);
    }

    return res;
}

ResultStatus fillStringVector(const std::string &str,
    std::vector<std::string> &stringVec, const int vecSize) {

    std::vector<std::string> tempString(vecSize);
    ResultStatus res = validateString(str, vecSize, tempString);
    if (!res.isError()) {
        stringVec = std::move(tempString);
    }

    return res;
}

ResultStatus fillVectors(std::vector<std::string> &stringVec,
    std::vector<int> &intVec,
    std::vector<float> &floatVec,
    const std::string &type,
    const std::string &str,
    int vecSize) {

    const uint64_t hashType = hashString(type.c_str());

    switch (hashType) {
        case hashString("int"):
            return fillIntVector(str, intVec, vecSize);

        case hashString("float"):
            return fillFloatVector(str, floatVec, vecSize);

        case hashString("string"):
            return fillStringVector(str, stringVec, vecSize);

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

void processInputVector(const std::string &type,
                      std::vector<std::string> &stringVec,
                      std::vector<int> &intVec,
                      std::vector<float> &floatVec, const int size)  {

    std::string inputStr;
    std::cout << "Input " << type << " vector: ";
    std::getline(std::cin, inputStr);
    const ResultStatus res = fillVectors(stringVec, intVec, floatVec, type, inputStr, size);
    logger(res);

    if (res.isError()) {
        std::cout << "Press enter: ";
        getchar();
    }
}

bool commandForVector(const uint64_t &hashCommand, std::string &type,
                    std::vector<std::string> &stringVec,
                    std::vector<int> &intVec, std::vector<float> &floatVec,
                    const int size) {

    switch (hashCommand) {

        case hashString("type"):
            system("clear");
            processInputType(type);
            return true;

        case hashString("input"):
            system("clear");
            processInputVector(type, stringVec, intVec, floatVec, size);
            return true;

        default:
            return false;
    }
}

void processInputName(std::string &name) {

    std::cout << "Input name: ";
    std::string inputStr;
    std::getline(std::cin, inputStr);
    if (inputStr.empty()) {
        logger(ResultStatus::Error("Input empty name."));
        std::cout << "Press enter: ";
        getchar();
    }else {
        name = std::move(inputStr);
    }
}

bool commandForUser(const uint64_t hashCommand, std::string &name) {

    switch (hashCommand) {
        case hashString("name"):
            system("clear");
            processInputName(name);
            return true;

        default:
            return false;
    }
}
