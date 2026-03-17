//
// Created by fisp on 01.03.2026.
//

#pragma once

#include <unordered_map>
#include <memory>

#include "AppSettings.h"
#include "DataPool.h"
#include "../../commonFunc/includes/functions.h"
#include "NetWork.h"
#include "SocketClient.h"


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
    IpAddress &ipAddress;
    std::string &type;
    uint64_t vecSize;

    bool sendToServer(const MyVec &vec) const;
    ResultStatus processIntVec(const std::vector<int64_t> &vec, SocketClient &client) const;
    ResultStatus processFloatVec(const std::vector<float> &vec, SocketClient &client) const;
    ResultStatus processStringVec(const std::vector<std::string> &vec, SocketClient &client) const;
public:
    explicit CommandVectorInput(DataPool<MyVec, size> &inputPool,
        std::string &typeNow, const uint64_t sizeOneVec, IpAddress &ipAddr)
        : pool(inputPool), ipAddress(ipAddr), type(typeNow), vecSize(sizeOneVec) {};

    void action() override;
};

template <uint64_t size>
ResultStatus CommandVectorInput<size>::processIntVec(const std::vector<int64_t> &vec, SocketClient &client) const {

    const PacketVector packetInt = createPacketVector(
                vec[0], vec[1], vec[2], vec[3]);

    if (!client.sendPacket(packetInt)) {
        return ResultStatus::Error("Failed to send packet.");
    }

    const Vec4 answer = client.receiveVectorFloat();

    std::cout << static_cast<int32_t> (answer.x) << " "
    << static_cast<int32_t> (answer.y) << " "
    << static_cast<int32_t> (answer.z) << " "
    << static_cast<int32_t> (answer.w) << std::endl;

    return ResultStatus::Good();
}

template <uint64_t size>
ResultStatus CommandVectorInput<size>::processFloatVec(const std::vector<float> &vec, SocketClient &client) const {

    const PacketVector packetFloat = createPacketVector(
                vec[0], vec[1], vec[2], vec[3]);

    if (!client.sendPacket(packetFloat)) {
        return ResultStatus::Error("Failed to send packet.");
    }

    const Vec4 answer = client.receiveVectorFloat();

    std::cout << answer.x << " " << answer.y << " " << answer.z << " " << answer.w << std::endl;

    return ResultStatus::Good();
}

template <uint64_t size>
ResultStatus CommandVectorInput<size>::processStringVec(const std::vector<std::string> &vec, SocketClient &client) const {

    const PacketString packetStr = createPacketString(
                vec[0] + vec[1] + vec[2] + vec[3]);

    if (!client.sendPacket(packetStr)) {
        return ResultStatus::Error("Failed to send packet.");
    }

    const int32_t answer = client.receiveInt();

    std::cout << "Size string: " << answer << std::endl;

    return ResultStatus::Good();
}

template <uint64_t size>
bool CommandVectorInput<size>::sendToServer(const MyVec &vec) const {
    if (!ipAddress.checkIpInput()) {
        logger(ResultStatus::Warning("Input ip (select this command in menu)."));
        return false;
    }

    SocketClient client(ipAddress.getIp(), ipAddress.getPort());

    if (!client.clientIsRunning()) {
        return false;
    }
    ResultStatus status;

    switch (hashString(vec.type.c_str())) {
        case hashString("int"):
            status = processIntVec(vec.intVec, client);
            break;

        case hashString("float"):
            status = processFloatVec(vec.floatVec, client);
            break;

        case hashString("string"):
            status = processStringVec(vec.stringVec, client);
            break;

        default:
            status = ResultStatus::Error("Type error.");
    }

    logger(status);

    if (status.isError()) {
        return false;
    }

    return true;
}

template <uint64_t size>
void CommandVectorInput<size>::action() {
    MyVec vec;
    vec.type = type;

    const ResultStatus res = processInputVector(vec.type, vec.stringVec,
        vec.intVec, vec.floatVec, vecSize);

    logger(res);

    if (res.isError() || !pool.insert(vec)) {
        std::cout << "Press enter to continue: ";
        getchar();
        return;
    }

    sendToServer(vec);
    std::cout << "Press enter to continue: ";
    getchar();

}

// Инвариант - appSettings — валидная ссылка на объект
class CommandCheckResource : public MenuItem {
    AppSettings &appSettings;
public:
    explicit CommandCheckResource(AppSettings &settings) : appSettings(settings) {};
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
