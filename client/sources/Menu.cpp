//
// Created by fisp on 01.03.2026.
//

#include "../includes/Menu.h"
#include "../includes/Tests.h"
#include "../../commonFunc/includes/ValidationFunction.h"

void CommandName::action() {
    std::string name;

    const ResultStatus res = processInputName(name);

    if (res.isGood()) {
        appSettings.setName(name);
    }
}

void CommandType::action() {
    processInputType(typeNow);
}

void CommandExit::action() {
    exit(0);
}

void Menu::addItem(const std::string &name, std::unique_ptr<MenuItem> item) {
    const uint64_t hashName = hashString(name.c_str());

    items[hashName] = std::move(item);
}

void Menu::findItem(const uint64_t &hashCommand) {
    auto result = items.find(hashCommand);

    if (result != items.end()) {
        result->second->action();
    }else {
        logger(RES_WARNING("Command not found."));
    }

}

std::vector<std::string> takeAndSplitStringToVector() {
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> params = split(input, ' ');

    return params;
};

void CommandCheckResource::action() {
    std::cout << "Resource test." << std::endl;
    std::cout << "Enter file names: ";

    const std::vector<std::string> params = takeAndSplitStringToVector();
    ResourceTest checkResource(params, appSettings.get().lib);

    system("clear");

    checkResource.test();

    std::cout << "Press any button to continue: ";
    getchar();
}

bool CommandIpAddress::workWithIpString(const std::string &input, ResultStatus &res) const {
    const std::vector<std::string> ipAndPort = split(input, ':');

    if (ipAndPort.size() != 2) {
        return false;
    }

    if (!(isIpAddress(ipAndPort[0]) && isPort(ipAndPort[1]))) {
        return false;
    }

    res = ipAddress.setAddress(input);

    return true;
}

bool CommandIpAddress::workWithVecIntString(const std::string &input, ResultStatus &res) const {
    std::vector<std::string> tempStringVec;
    std::vector<int64_t> tempIntVec;
    std::vector<float> tempFloatVec;

    fillVectors(tempStringVec, tempIntVec,
        tempFloatVec, "int", input, 5);

    if (tempIntVec.size() != 5) {
        return false;
    }

    res = ipAddress.setAddress(tempIntVec);
    return true;
}

bool CommandIpAddress::workWithHexString(const std::string &input, ResultStatus &res) const {
    if (!hexStringIsIpWithPort(input)) {
        return false;
    }

    const uint64_t hex = hexStringToUint64_t(input);
    res = ipAddress.setAddress(hex);
    return true;
}

void CommandIpAddress::action() {
    std::cout << "Input format 1.1.1.1:5345 or 1 1 1 1 5345 or 0x0101010114E1." << std::endl;
    std::cout << "My server work on 127.0.0.1:8080." << std::endl;
    std::string input;
    std::getline(std::cin, input);
    input = stringStrip(input);

    ResultStatus res;
    const bool status = workWithIpString(input, res) ||
            workWithVecIntString(input, res) ||
            workWithHexString(input, res);

    if (!status) {
        logger(RES_ERROR("Input incorrect address: " + input));
        std::cout << "Press any key to continue: ";
        getchar();
        return;
    }

    const std::vector<std::string> inputParams {ipAddress.getAddress()};
    ConnectionTest connection{inputParams};
    if (!connection.test()) {
        logger(RES_ERROR("Cant connect to address: " + input));
        std::cout << "Press any key to continue: ";
        getchar();
        return;
    }

}

void printMenu(const std::string &name) {
    system("clear");
    std::cout << "Hi, " << name << "." << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << "name - input name" << std::endl;
    std::cout << "type - input type vector" << std::endl;
    std::cout << "input - input vector value and multi him on server" << std::endl;
    std::cout << "filecheck - check files in dict (from flag -l)" << std::endl;
    std::cout << "show - show last vector, current type and address connect" << std::endl;
    std::cout << "ip - input ip and port to connect" << std::endl;
    std::cout << "exit/quit - close program" << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << "Command: ";
}


