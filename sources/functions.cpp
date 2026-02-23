//
// Created by fisp on 23.02.2026.
//
#include "../includes/functions.h"

bool isInteger(const std::string &integer) {
    size_t lastPosition;
    try {
        stoi(integer, &lastPosition);
        if (lastPosition != integer.length()) {
            return false;
        }
        return true;
    }catch (...) {
        return false;
    }
}

bool isFloat(const std::string &value) {
    size_t lastPosition;
    try {
        stof(value, &lastPosition);
        if (lastPosition != value.length()) {
            return false;
        }
        return true;
    }catch (...) {
        return false;
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
        if (!isInteger(byte)) {
            return false;
        }

        int32_t num = stoi(byte);
        if (num < 0 || num > 255) {
            return false;
        }
        count++;
    }

    if (count != 4) {
        return false;
    }

    return true;
}

bool isPort(const std::string &port) {
    uint32_t portValue = stoi(port);

    if (portValue <= 65535) {
        return true;
    }
    return false;
}

void processingFlag(const uint64_t flag, Config &config, const std::string& argFlag) {
    switch (flag) {

        case hashString("-a"):
            if (isIpAddress(argFlag)) {
                config.address = argFlag;
            }else {
                std::cerr << "Invalid address: " << argFlag << std::endl;
                exit(-1);
            }
            break;

        case hashString("-p"):
            if (isInteger(argFlag) && isPort(argFlag)) {
                config.port = stoi(argFlag);

            }else {
                std::cerr << "Invalid port(0-65535): " << argFlag << std::endl;
                exit(-1);
            }
            break;

        case hashString("-r"):
            config.role = argFlag;
            break;

        case hashString("-i"):
            if (isInteger(argFlag)) {
                config.id = stoi(argFlag);

            }else {
                std::cerr << "Invalid id: " << argFlag << std::endl;
                exit(-1);
            }
            break;

        case hashString("-L"):
            config.lib = argFlag;
            break;

        default:
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

std::string lowerString(std::string str) {
    for (char &ch : str) {
        ch = static_cast<char>(std::tolower(ch));
    }

    return str;
}

bool fillVector(std::vector<std::string> &stringVec,
    std::vector<int> &intVec,
    std::vector<float> &floatVec,
    const std::string &type,
    const std::string &str,
    const int &vecSize) {

    std::vector<int> tempInt(vecSize);
    std::vector<float> tempFloat(vecSize);
    std::vector<std::string> tempString(vecSize);

    int count = 0;

    uint64_t hashType = hashString(type.c_str());

    std::istringstream ss(str);
    std::string element;
    while (getline(ss, element, ' ')) {
        if (count == vecSize) {
            break;
        }
        switch (hashType) {
            case hashString("int"):
                if (!isInteger(element)) {
                    return false;
                }
                tempInt[count] = stoi(element);
                break;

            case hashString("float"):
                if (!isFloat(element)) {
                    return false;
                }
                tempFloat[count] = stof(element);
                break;

            case hashString("string"):
                tempString[count] = element;
                break;

            default:;
        }
        count++;
    }

    if (count != vecSize) {
        return false;
    }

    switch (hashType) {
        case hashString("int"):
            intVec = tempInt;
            break;
        case hashString("float"):
            floatVec = tempFloat;
            break;
        case hashString("string"):
            stringVec = tempString;
            break;
        default: ;
    }

    return true;
}
