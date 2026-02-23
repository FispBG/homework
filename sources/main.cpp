#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "../includes/functions.h"

#define VEC_SIZE 4

// constexpr uint64_t hashString(const char* str) {
//     uint64_t hash = 0;
//
//     while (*str) {
//         hash = hash * 31 + static_cast<uint64_t>(*str);
//         str++;
//     }
//     return hash;
// }

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

int main(int argc, const char **argv) {
    Config config = createConfig(argc, argv);

    std::string name = "Guest";
    std::string type = "string";

    std::vector<std::string> stringVec(VEC_SIZE);
    std::vector<int> intVec(VEC_SIZE);
    std::vector<float> floatVec(VEC_SIZE);

    while (true) {
        printMenu(name);
        std::cout << "Your type: " << type << std::endl;
        std::cout << "Your vectors" << std::endl;
        printVector(stringVec, VEC_SIZE, "String");
        printVector(intVec, VEC_SIZE, "Int");
        printVector(floatVec, VEC_SIZE, "Float");
        std::cout << "-------------------" << std::endl;
        std::cout << "Command: ";

        std::string command;
        std::getline(std::cin, command);
        command = lowerString(command);
        const uint64_t hashCommand = hashString(command.c_str());
        std::string inputStr;

        switch (hashCommand) {
            case hashString("name"):
                system("clear");
                std::cout << "Input name: ";
                std::getline(std::cin, name);
                break;

            case hashString("type"):
                system("clear");
                std::cout << "Input type(int, float, string): ";
                std::getline(std::cin, inputStr);

                inputStr = lowerString(inputStr);
                if (inputStr != "string" && inputStr != "int" && inputStr != "float") {
                    std::cout << "Invalid type: " << inputStr << std::endl;
                    std::cout << "Press enter: ";
                    getchar();
                }else {
                    type = inputStr;
                }

                break;

            case hashString("input"):
                system("clear");
                std::cout << "Input " << type << " vector: ";
                std::getline(std::cin, inputStr);
                if (!fillVector(stringVec, intVec, floatVec, type, inputStr, VEC_SIZE)) {
                    std::cout << "Invalid input(type)" << std::endl;
                    std::cout << "Press enter: ";
                    getchar();
                }

                break;

            case hashString("exit"):
                return 0;

            default: ;
        }
    }
}