#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>

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
    }catch (std::invalid_argument& e) {
        return false;
    }
}

bool isIpAddress(const std::string &ip) {
    std::stringstream ss(ip);
    std::string byte;
    std::vector<int16_t> ipBytes;
    while (std::getline(ss, byte, '.')) {
        if (isInteger(byte)) {

            int16_t num = stoi(byte);
            if (num >= 0 && num <= 255) {
                ipBytes.push_back(num);
            }else {
                return false;
            }

        }else {
            return false;
        }
    }

    if (ipBytes.size() != 4) {
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

Config createConfig(const int &argc, char *argv[]) {
    Config config;

    for (int i = 1; i < argc; i++) {
        std::string flag = argv[i];
        std::string argFlag;
        std::cout << "flag: " << flag << std::endl;

        if (i+1 >= argc) {
            std::cerr << "No argument for flag: " << argFlag << std::endl;
            exit(-1);
        }

        argFlag = argv[i+1];

        if (argFlag[0] == '-') {
            std::cerr << "Invalid arg flag: " << argFlag << std::endl;
            exit(-1);
        }

        if (flag == "-a") {

            if (isIpAddress(argFlag)) {
                config.address = argFlag;
                i++;
            }else {
                std::cerr << "Invalid address: " << argFlag << std::endl;
                exit(-1);
            }

        }else if (flag == "-p") {

            if (isInteger(argFlag) && isPort(argFlag)) {
                config.port = stoi(argFlag);
                i++;
            }else {
                std::cerr << "Invalid port(0-65535): " << argv[i+1] << std::endl;
                exit(-1);
            }

        }else if (flag == "-r") {

            config.role = argFlag;
            i++;

        }else if (flag == "-i") {

            if (isInteger(argFlag)) {
                int32_t id = stoi(argFlag);
                config.id = id;
                i++;
            }else {
                std::cerr << "Invalid id: " << argFlag << std::endl;
                exit(-1);
            }

        }else if (flag == "-L") {
            config.lib = std::string(argv[i+1]);
            i++;

        }else {
            std::cerr << "Invalid key: " << flag << std::endl;
            exit(-1);
        }
    }

    return config;
}

void printMenu() {
    system("clear");
    std::cout << "name - input name" << std::endl;
    std::cout << "type - input type vector" << std::endl;
    std::cout << "input - input vector value" << std::endl;
    std::cout << "exit - close program" << std::endl;
}

int main(int argc, char** argv) {
    // std::string address = "1.0.100.190";
    // std::cout << stoi(address);
    // int position = 0;
    // int dotCount = 0;
    // int indexDot = address.find('.', position);
    //

    // while (indexDot != -1) {
    //     int oneByte = std::stoi(address.substr(position, indexDot - position));
    //
    //     if (!(oneByte >= 0 && oneByte <= 255)) {
    //         std::cerr << "Invalid byte: " << oneByte << std::endl;
    //         return -1;
    //     }
    //
    //     position = indexDot + 1;
    //     dotCount++;
    //     indexDot = address.find('.', position);
    //
    // }
    // int index = address.find(".", 6);
    // std::cout << index << std::endl;
    // for (int i =0; i < argc; i++) {
    //     std::cout << argv[i] << std::endl;
    // }

    Config config = createConfig(argc, argv);

    std::cout << config.address << std::endl;
    std::cout << config.port << std::endl;
    std::cout << config.role << std::endl;
    std::cout << config.id << std::endl;
    std::cout << config.lib << std::endl;
    std::cout << std::stoi("55f5") << std::endl;
    std::string name = "Guest";
    std::string type;

    // while (true) {
    //     printMenu();
    //     std::string command;
    //     std::cin >> command;
    //
    //     if (command == "name") {
    //         system("clear");
    //         std::cout << "Input name: ";
    //         std::cin >> name;
    //     }else if (command == "type") {
    //         system("clear");
    //         std::cout << "Input type: ";
    //         std::cin >> type;
    //
    //         if (type == "int") {
    //
    //         }
    //
    //     }else if (command == "exit") {
    //         return 0;
    //     }
    // }
    return 0;
}