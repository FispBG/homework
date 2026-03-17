//
// Created by fisp on 02.03.2026.
//

#include "../includes/Tests.h"
#include "../../commonFunc/includes/functions.h"
#include "../includes/SocketClient.h"
#include <filesystem>

bool ConnectionTest::test() {
    std::cout << "Connection test" << std::endl;
    if (params.size() <= 0) {
        std::cout << "No parameters" << std::endl;
        return false;
    }
    const std::vector<std::string> ipAndPort = split(params[0], ':');

    if (ipAndPort.size() != 2) {
        std::cout << "Invalid address: " << params[0] << std::endl;
        return false;
    }
    if (!isPort(ipAndPort[1])) {
        std::cout << "Invalid port number: " << ipAndPort[1] << std::endl;
        return false;
    }

    SocketClient client(ipAndPort[0], std::stoi(ipAndPort[1]));

    if (!client.clientIsRunning()) {
        return false;
    }
    std::cout << "Address is good." << std::endl;
    return true;

}

bool ResourceTest::test() {
    for (auto elem: params) {
        std::filesystem::path path = std::filesystem::path(lib) / elem;

        if (std::filesystem::exists(path)) {
            std::cout << "File exists: " << path << std::endl;
        }else {
            std::cout << "File does not exist: " << path << std::endl;
            return false;
        }
    }

    return true;
}