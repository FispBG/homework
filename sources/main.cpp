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

#define VEC_SIZE 4
#define POOL_SIZE 5

int main(const int argc, const char **argv) {
    AppSettings settings;

    ResultStatus configCreate = settings.createConfig(argc, argv);

    if (configCreate.isError()) {
        exit(-1);
    }

    DataPool<MyVec, POOL_SIZE> pool;
    Menu menu;
    IpAddress ipConnect;
    std::string type = "string";

    menu.addItem("name", std::make_unique<CommandName>(settings));
    menu.addItem("input", std::make_unique<CommandVectorInput<POOL_SIZE>>
        (pool, type, VEC_SIZE));

    menu.addItem("type", std::make_unique<CommandType>(type));
    menu.addItem("test", std::make_unique<CommandTest>(settings));
    menu.addItem("show", std::make_unique<CommandShow<POOL_SIZE>>
        (pool, type, ipConnect, VEC_SIZE));

    menu.addItem("ip",  std::make_unique<CommandIpAddress>(ipConnect));
    menu.addItem("exit", std::make_unique<CommandExit>());
    menu.addItem("quit", std::make_unique<CommandExit>());

    while (true) {
        printMenu(settings.getName());

        std::string command;
        std::getline(std::cin, command);

        command = fixInputString(command);
        uint64_t hashCommand = hashString(command.c_str());

        system("clear");
        menu.findItem(hashCommand);
    }
}