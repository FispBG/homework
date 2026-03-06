#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "../includes/AppSettings.h"
#include "../includes/DataPool.h"
#include "../includes/Menu.h"
#include "../includes/NetWork.h"

#define VEC_SIZE 4

int main(const int argc, const char **argv) {
    AppSettings settings;

    ResultStatus configCreate = settings.createConfig(argc, argv);

    if (configCreate.isError()) {
        return -1;
    }

    DataPool pool(2);
    Menu menu;
    IpAddress ipConnect;
    std::string type = "string";

    menu.addItem("name", new CommandName(settings));
    menu.addItem("input", new CommandVectorInput(pool, type, VEC_SIZE));
    menu.addItem("type", new CommandType(type));
    menu.addItem("test", new CommandTest(settings));
    menu.addItem("show", new CommandShow(pool, type, VEC_SIZE, ipConnect));
    menu.addItem("ip", new CommandIpAddress(ipConnect));
    menu.addItem("exit", new CommandExit());

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