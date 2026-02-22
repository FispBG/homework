#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
#include <string>

struct Config{
    std::string address;
    int32_t port = 0;
    std::string role;
    int32_t id = 0;
    std::string lib;
};

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

    if (count!= 4) {
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

constexpr uint64_t hashString(const char* str) {
    uint64_t hash = 0;

    while (*str) {
        hash = hash * 31 + *str;
        str++;
    }
    return hash;
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
        const uint32_t flag = hashString(argv[i]);

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

std::string lowerString(std::string s) {
    for (char &ch : s) {
        ch = static_cast<char>(std::tolower(ch));
    }

    return s;
}

void printMenu(const std::string &name) {
    system("clear");
    std::cout << "Hi, " << name << "." << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << "name - input name" << std::endl;
    std::cout << "type - input type vector" << std::endl;
    std::cout << "input - input vector value" << std::endl;
    std::cout << "exit - close program" << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << "Command: ";
}

int main(int argc, const char **argv) {
    Config config = createConfig(argc, argv);

    std::string name = "Guest";
    std::string type;

    while (true) {
        printMenu(name);
        std::string command;
        std::getline(std::cin, command);
        uint64_t hashCommand = hashString(command.c_str());

        switch (hashCommand) {
            case hashString("name"):
                system("clear");
                std::cout << "Input name: ";
                std::getline(std::cin, name);
                break;

            case hashString("type"):
                system("clear");
                std::cout << "Input type: ";
                std::getline(std::cin, type);
                break;

            case hashString("input"):
                break;

            case hashString("exit"):
                return 0;

            default: ;
        }
    }
}