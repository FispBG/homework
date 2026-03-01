//
// Created by fisp on 01.03.2026.
//

#ifndef HOMEWORK_MENU_H
#define HOMEWORK_MENU_H
#include <unordered_map>

#include "AppSettings.h"
#include "DataPool.h"
#include "functions.h"

class MenuItem {
public:
    virtual void action() = 0;
    virtual ~MenuItem() = default;
};

// Инвариант - appSettings — валидная ссылка на объект
class CommandName : public MenuItem {
    AppSettings &appSettings;

public:
    explicit CommandName( AppSettings &appSettings) : appSettings(appSettings) {}

    void action() override;
};

// Инвариант - typeNow один из: int, float, string
class CommandType : public MenuItem {
    std::string &typeNow;

public:
    explicit CommandType(std::string &typeNow) : typeNow(typeNow) {};
    void action() override;
};

// Инвариант - pool — валидная ссылка на объект
// size > 0, иначе не смысла от объекта
class CommandVectorInput : public MenuItem {
    DataPool &pool;
    std::string &type;
    int size = 0;
public:
    explicit CommandVectorInput(DataPool &inputPool,
        std::string &typeNow, const int size) : pool(inputPool), type(typeNow), size(size) {};

    void action() override;
};

// Инвариант - appSettings — валидная ссылка на объект
class CommandTest : public MenuItem {
    AppSettings &appSettings;

public:
    explicit CommandTest(AppSettings &settings) : appSettings(settings) {};
    void action() override;
};

class CommandExit : public MenuItem {
public:
    explicit CommandExit() : MenuItem() {};
    void action() override;
};

// Инвариант - pool — валидная ссылка на объект
// vecSize > 0, иначе не смысла от объекта
class CommandShow : public MenuItem {
    DataPool &pool;
    std::string &type;
    int vecSize = 0;
public:
    explicit CommandShow(DataPool &pool, std::string &type,
        const int size) : pool(pool), type(type), vecSize(size) {};

    void action() override;

};


// Инвариант - уникальные ключи в items
// Указатели в items не nullptr
class Menu {
    std::unordered_map<uint64_t, std::unique_ptr<MenuItem>> items;

public:
    void addItem(const std::string &name, MenuItem *item);
    void findItem(const uint64_t &hashCommand);
};

void printMenu(const std::string &name);

#endif //HOMEWORK_MENU_H