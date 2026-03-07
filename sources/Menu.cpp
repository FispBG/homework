//
// Created by fisp on 01.03.2026.
//

#include "../includes/Menu.h"
#include "../includes/Tests.h"

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

void CommandVectorInput::action() {
    MyVec vec;
    vec.type = type;

    const ResultStatus res = processInputVector(vec.type, vec.stringVec,
        vec.intVec, vec.floatVec, size);

    if (!res.isError()) {
        if (!pool.insert(vec)) {
            std::cout << "Press enter to continue: ";
            getchar();
        }
    }
}

void CommandExit::action() {
    exit(0);
}

void Menu::addItem(const std::string &name, MenuItem *item) {
    const uint64_t hashName = hashString(name.c_str());

    items[hashName] = std::unique_ptr<MenuItem>(item);
}

void Menu::findItem(const uint64_t &hashCommand) {
    auto result = items.find(hashCommand);

    if (result != items.end()) {
        result->second->action();
    }else {
        logger(ResultStatus::Warning("Command not found."));
    }

}

std::vector<std::string> takeAndSplitStringToVector() {
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> params = split(input, ' ');

    return params;
};


void CommandTest::action() {
    std::vector<std::unique_ptr<Test>> vecTests;
    std::vector<std::string> params;

    std::cout << "Tests" << std::endl;
    std::cout << "---------------------" << std::endl;
    std::cout << "Connection test" << std::endl;
    std::cout << "Enter connection params(1.2.3.4 43243): ";

    params = takeAndSplitStringToVector();
    vecTests.push_back(std::make_unique<ConnectionTest>(params));

    std::cout << "---------------------" << std::endl;
    std::cout << "Resource test" << std::endl;
    std::cout << "Enter file names: ";

    params = takeAndSplitStringToVector();
    vecTests.push_back(std::make_unique<ResourceTest>(params, appSettings.getLib()));

    std::cout << "---------------------" << std::endl;
    system("clear");

    for (const auto &it : vecTests) {
        it->test();
        std::cout << "---------------------" << std::endl;
    }

    std::cout << "Press enter to continue: ";
    getchar();
}

void CommandShow::action() {
    std::cout << "-------------------" << std::endl;
    std::cout << "Your vector:" << std::endl;
    if (pool.empty()) {
        logger(ResultStatus::Warning("No one vector input."));
    }else {
        MyVec vec = pool.last();
        switch (hashString(vec.type.c_str())) {
            case(hashString("int")):
                printVector(vec.intVec, vecSize, vec.type);
                break;

            case(hashString("float")):
                printVector(vec.floatVec, vecSize, vec.type);
                break;

            case(hashString("string")):
                printVector(vec.stringVec, vecSize, vec.type);
                break;

            default:;
        }
    }

    std::cout << "Your current type: " << type << std::endl;
    std::cout << "Your current ip: " << ipAddress << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << "Press enter to continue: ";
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

    res = ipAddress.setIpAddress(input);

    return true;
}

bool CommandIpAddress::workWithVecIntString(const std::string &input, ResultStatus &res) const {
    std::vector<int64_t> intVec;
    fillIntVector(input, intVec, 5);

    if (intVec.size() != 5) {
        return false;
    }

    res = ipAddress.setIpAddress(intVec);
    return true;
}

bool CommandIpAddress::workWithHexString(const std::string &input, ResultStatus &res) const {
    if (!hexStringIsIpWithPort(input)) {
        return false;
    }

    const uint64_t hex = hexStringToUint64_t(input);
    res = ipAddress.setIpAddress(hex);
    return true;
}

void CommandIpAddress::action() {
    std::cout << "Input format 1.1.1.1:5345 or 1 1 1 1 5345 or 0x0101010114E1." << std::endl;
    std::string input;
    std::getline(std::cin, input);
    input = stringStrip(input);

    ResultStatus res;
    const bool status = workWithIpString(input, res) ||
            workWithVecIntString(input, res) ||
            workWithHexString(input, res);

    if (!status) {
        res = ResultStatus::Error("Input incorrect address: " + input);
    }

    logger(res);

    if (!res.isGood()) {
        std::cout << "Press enter to continue: ";
        getchar();
    }
}

void printMenu(const std::string &name) {
    system("clear");
    std::cout << "Hi, " << name << "." << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << "name - input name" << std::endl;
    std::cout << "type - input type vector" << std::endl;
    std::cout << "input - input vector value" << std::endl;
    std::cout << "test - check connections and files" << std::endl;
    std::cout << "show - show last vector and current type" << std::endl;
    std::cout << "ip - input ip to connect" << std::endl;
    std::cout << "exit - close program" << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << "Command: ";
}


