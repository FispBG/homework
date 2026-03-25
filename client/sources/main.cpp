#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>
#include <string>

#include "../includes/AppSettings.h"
#include "../includes/DataPool.h"
#include "../includes/Menu.h"
#include "../includes/NetWork.h"

constexpr size_t VEC_SIZE = 4;
constexpr size_t POOL_SIZE = 5;

int main(const int argc, const char **argv) {
    IpAddress ipConnect;
    AppSettings settings(ipConnect);

    ResultStatus configCreate = settings.createConfig(argc, argv);

    if (configCreate.isError()) {
        exit(-1);
    }

    DataPool<MyVec, POOL_SIZE> pool;
    Menu menu;
    std::string type = "string";

    menu.addItem("name", std::make_unique<CommandName>(settings));
    menu.addItem("input", std::make_unique<CommandVectorInput<POOL_SIZE>>
        (pool, type, VEC_SIZE, ipConnect));

    menu.addItem("type", std::make_unique<CommandType>(type));
    menu.addItem("filecheck", std::make_unique<CommandCheckResource>(settings));
    menu.addItem("show", std::make_unique<CommandShow<POOL_SIZE>>
        (pool, type, ipConnect, VEC_SIZE));

    menu.addItem("ip",  std::make_unique<CommandIpAddress>(ipConnect));
    menu.addItem("exit", std::make_unique<CommandExit>());
    menu.addItem("quit", std::make_unique<CommandExit>());

    while (true) {
        printMenu(settings.get().name);

        std::string command;
        std::getline(std::cin, command);

        command = fixInputString(command);
        uint64_t hashCommand = hashString(command.c_str());

        system("clear");
        menu.findItem(hashCommand);
    }
}

