//
// Created by fisp on 02.03.2026.
//

#include "../includes/Tests.h"
#include "../includes/functions.h"
#include <filesystem>

void ConnectionTest::test() {
    std::cout << "Connection test" << std::endl;
    for (auto elem: params) {
        if (isIpAddress(elem)) {
            std::cout << "Normal ip: " << elem << std::endl;
        }else if (isPort(elem)) {
            std::cout << "Normal port: " << elem << std::endl;
        }else {
            std::cout << "Invalid param: " << elem << std::endl;
        }
    }
}

void ResourceTest::test() {
    for (auto elem: params) {
        std::filesystem::path path = std::filesystem::path(lib) / elem;

        if (std::filesystem::exists(path)) {
            std::cout << "File exists: " << path << std::endl;
        }else {
            std::cout << "File does not exist: " << path << std::endl;
        }
    }
}