#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "../includes/functions.h"

#define VEC_SIZE 4

void printMenu(const std::string &name) {

    system("clear");
    std::cout << "Hi, " << name << "." << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << "name - input name" << std::endl;
    std::cout << "type - input type vector" << std::endl;
    std::cout << "input - input vector value" << std::endl;
    std::cout << "exit - close program" << std::endl;
    std::cout << "-------------------" << std::endl;
}

void printVariables(const std::vector<std::string> &stringVec,
    const std::vector<int> &intVec,
    const std::vector<float> &floatVec,
    const std::string &type) {

    std::cout << "-------------------" << std::endl;
    std::cout << "Your type: " << type << std::endl;
    std::cout << "Your vectors" << std::endl;
    printVector(stringVec, VEC_SIZE, "String");
    printVector(intVec, VEC_SIZE, "Int");
    printVector(floatVec, VEC_SIZE, "Float");
    std::cout << "-------------------" << std::endl;
}

int main(const int argc, const char **argv) {
    Config config;
    ResultStatus configCreate = createConfig(argc, argv, config);
    if (configCreate.isError()) {
        return -1;
    }

    std::string name = "Guest";
    std::string type = "string";

    std::vector<std::string> stringVec(VEC_SIZE);
    std::vector<int> intVec(VEC_SIZE);
    std::vector<float> floatVec(VEC_SIZE);

    while (true) {
        printMenu(name);
        printVariables(stringVec, intVec, floatVec, type);
        std::cout << "Command: ";

        std::string command;
        std::getline(std::cin, command);

        command = fixInputString(command);
        const uint64_t hashCommand = hashString(command.c_str());

        commandForVector(hashCommand, type, stringVec, intVec, floatVec, VEC_SIZE);
        commandForUser(hashCommand, name);

        switch (hashCommand) {
            case hashString("exit"):
                return 0;

            default: ;
        }
    }
}