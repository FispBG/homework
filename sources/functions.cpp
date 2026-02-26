//
// Created by fisp on 23.02.2026.
//
#include "../includes/functions.h"

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

std::string lowerString(std::string str) {
    for (char &ch : str) {
        ch = static_cast<char>(std::tolower(ch));
    }

    return str;
}

void processOnlyIp(Config &config, const std::string &ip) {
    if (isIpAddress(ip)) {
        config.address = ip;
    }else {
        std::cerr << "Invalid address: " << ip << std::endl;
        exit(-1);
    }
}

void processOnlyPort(Config &config, const std::string &port) {
    if (isPort(port)) {
        config.port = strtol(port.c_str(), nullptr, 10);

    }else {
        std::cerr << "Invalid port(0-65535): " << port << std::endl;
        exit(-1);
    }
}

void processOnlyId(Config &config, const std::string &id) {
    char* firstNotInt = nullptr;
    const int valueId = strtol(id.c_str(), &firstNotInt, 10);

    if (*firstNotInt != '\0') {
        std::cerr << "Invalid id: " << id << std::endl;
        exit(-1);
    }

    config.id = valueId;
}

bool processNetworkFlags(const uint64_t flag, Config &config, const std::string &argFlag) {
    switch (flag) {
        case hashString("-a"):
            processOnlyIp(config, argFlag);
            return true;

        case hashString("-p"):
            processOnlyPort(config, argFlag);
            return true;

        default:
            return false;
    }
}

bool processUserFlags(const uint64_t flag, Config &config, const std::string &argFlag) {
    switch (flag) {
        case hashString("-r"):
            config.role = argFlag;
            return true;

        case hashString("-i"):
            processOnlyId(config, argFlag);
            return true;

        default:
            return false;
    }
}

bool processAppFlags(const uint64_t flag, Config &config, const std::string &argFlag) {
    switch (flag) {
        case hashString("-L"):
            config.lib = argFlag;
            return true;

        default:
            return false;
    }
}

void processingFlag(const uint64_t flag, Config &config, const std::string& argFlag) {

    bool resultProcessing = processNetworkFlags(flag, config, argFlag) ||
                            processUserFlags(flag, config, argFlag) ||
                            processAppFlags(flag, config, argFlag);

    if (!resultProcessing) {
        std::cerr << "Invalid key: " << flag << std::endl;
        exit(-1);
    }
}

Config createConfig(const int &argc, const char *argv[]) {
    Config config;

    for (int i = 1; i < argc; i++) {
        const uint64_t flag = hashString(argv[i]);

        if (i+1 >= argc) {
            std::cerr << "No argument for flag: " << argv[i] << std::endl;
            exit(-1);
        }

        const std::string argFlag = argv[++i];

        if (argFlag[0] == '-') {
            std::cerr << "Invalid arg flag: " << argFlag << std::endl;
            exit(-1);
        }

        processingFlag(flag, config, argFlag);
    }

    return config;
}

template<typename T, typename Transform>
bool validateNumberString(const std::string &str, std::vector<T> &vecNumbers,
    const int vecSize, Transform transform) {

    std::istringstream ss(str);
    std::string element;
    int count = 0;

    while (count < vecSize && getline(ss, element, ' ')) {
        char* firstNotInt = nullptr;
        vecNumbers[count] = transform(element.c_str(), &firstNotInt);
        if (*firstNotInt != '\0') {
            return false;
        }
        count++;
    }

    return count == vecSize;
}

bool validateString(const std::string &str, const int vecSize,
    std::vector<std::string> &vecString) {

    std::istringstream ss(str);
    std::string element;
    int count = 0;

    while (count < vecSize && getline(ss, element, ' ')) {
        vecString[count] = element;
        count++;
    }

    return count == vecSize;
}

long stringToLongDec(const char* str, char** end) {
    return std::strtol(str, end, 10);
}

bool fillIntVector(const std::string &str,
    std::vector<int> &intVec, const int vecSize) {

    std::vector<int> tempInt(vecSize);
    if (!validateNumberString(str, tempInt, vecSize, stringToLongDec)) {
        return false;
    }

    intVec = tempInt;
    return true;
}

bool fillFloatVector(const std::string &str,
    std::vector<float> &floatVec, const int vecSize) {

    std::vector<float> tempFloat(vecSize);
    if (!validateNumberString(str, tempFloat, vecSize, strtod)) {
        return false;
    }

    floatVec = tempFloat;
    return true;
}

bool fillStringVector(const std::string &str,
    std::vector<std::string> &stringVec, const int vecSize) {

    std::vector<std::string> tempString(vecSize);
    if (!validateString(str, vecSize, tempString)) {
        return false;
    }

    stringVec = tempString;
    return true;
}

bool fillVectors(std::vector<std::string> &stringVec,
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

        default:;
    }

    return true;
}

void processInputType(std::string &type)  {

    std::string inputStr;
    std::cout << "Input type(int, float, string): ";
    std::getline(std::cin, inputStr);

    inputStr = lowerString(inputStr);
    if (inputStr != "string" && inputStr != "int" && inputStr != "float") {
        std::cout << "Invalid type: " << inputStr << std::endl;
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

    if (!fillVectors(stringVec, intVec, floatVec, type, inputStr, size)) {
        std::cout << "Invalid input(type)" << std::endl;
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
    std::getline(std::cin, name);
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
