//
// Created by fisp on 01.03.2026.
//

#ifndef HOMEWORK_MENU_H
#define HOMEWORK_MENU_H

#include <unordered_map>
#include <memory>

#include "AppSettings.h"
#include "DataPool.h"
#include "functions.h"
#include "NetWork.h"


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
// size > 0
template <uint64_t size>
class CommandVectorInput : public MenuItem {
    DataPool<MyVec, size> &pool;
    std::string &type;
    uint64_t vecSize;

public:
    explicit CommandVectorInput(DataPool<MyVec, size> &inputPool,
        std::string &typeNow, const uint64_t sizeOneVec)
        : pool(inputPool), type(typeNow), vecSize(sizeOneVec) {};

    void action() override;
};

template <uint64_t size>
void CommandVectorInput<size>::action() {
    MyVec vec;
    vec.type = type;

    const ResultStatus res = processInputVector(vec.type, vec.stringVec,
        vec.intVec, vec.floatVec, vecSize);

    if (!res.isError()) {
        if (!pool.insert(vec)) {
            std::cout << "Press enter to continue: ";
            getchar();
        }
    }
}

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
// vecSize > 0
template <uint64_t size>
class CommandShow : public MenuItem {
    DataPool<MyVec, size> &pool;
    std::string &type;
    IpAddress &ipAddress;
    uint64_t vecSize;

public:
    explicit CommandShow(DataPool<MyVec, size> &pool,
        std::string &type, IpAddress &ipAddr, const uint64_t sizeOneVec) :
        pool(pool), type(type), ipAddress(ipAddr), vecSize(sizeOneVec) {};

    void action() override;
};

template <uint64_t size>
void CommandShow<size>::action() {
    std::cout << "-------------------" << std::endl;
    std::cout << "Your vector:" << std::endl;
    if (pool.empty()) {
        logger(ResultStatus::Warning("No one vector input."));
    }else {
        MyVec vec = pool.last();
        switch (hashString(vec.type.c_str())) {
            case(hashString("int")):
                printVector(vec.intVec, size, vec.type);
                break;

            case(hashString("float")):
                printVector(vec.floatVec, size, vec.type);
                break;

            case(hashString("string")):
                printVector(vec.stringVec, size, vec.type);
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

class CommandIpAddress : public MenuItem {
    IpAddress &ipAddress;

    bool workWithIpString(const std::string &input, ResultStatus &res) const;
    bool workWithVecIntString(const std::string &input, ResultStatus &res) const;
    bool workWithHexString(const std::string &input, ResultStatus &res) const;

public:
    explicit CommandIpAddress(IpAddress &ipAddress) : ipAddress(ipAddress) {};

    void action() override;
};

// Инвариант - уникальные ключи в items
// Указатели в items не nullptr
class Menu {
    std::unordered_map<uint64_t, std::unique_ptr<MenuItem>> items;

public:
    void addItem(const std::string &name, std::unique_ptr<MenuItem> item);
    void findItem(const uint64_t &hashCommand);
};

void printMenu(const std::string &name);

#endif //HOMEWORK_MENU_H