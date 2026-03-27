//
// Created by fisp on 01.03.2026.
//

#pragma once

#include <unordered_map>
#include <memory>

#include "../../commonFunc/includes/PacketFunction.h"
#include "../../commonFunc/includes/VectorProcess.h"
#include "AppSettings.h"
#include "DataPool.h"
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

    bool sendToServer(MyVec &vec) const;
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
        return RES_ERROR("Failed to send packet.");
    }

    const std::vector<int32_t> answer = client.receiveVector<int32_t>();

    std::cout << answer[0] << " " << answer[1] << " " << answer[2] << " " << answer[3] << std::endl;

    return ResultStatus::Good();
}

template <uint64_t size>
ResultStatus CommandVectorInput<size>::processFloatVec(const std::vector<float> &vec, SocketClient &client) const {

    const PacketVector packetFloat = createPacketVector(
                vec[0], vec[1], vec[2], vec[3]);

    if (!client.sendPacket(packetFloat)) {
        return RES_ERROR("Failed to send packet.");
    }

    const std::vector<float> answer = client.receiveVector<float>();

    std::cout << answer[0] << " " << answer[1] << " " << answer[2] << " " << answer[3] << std::endl;

    return ResultStatus::Good();
}

template <uint64_t size>
ResultStatus CommandVectorInput<size>::processStringVec(const std::vector<std::string> &vec, SocketClient &client) const {

    const PacketString packetStr = createPacketString(
                vec[0] + " " + vec[1] + " " + vec[2] + " " + vec[3]);

    if (!client.sendPacket(packetStr)) {
        return RES_ERROR("Failed to send packet.");
    }

    const std::vector<int32_t> answer = client.receiveVector<int32_t>();

    std::cout << answer[0] << " " << answer[1] << " " << answer[2] << " " << answer[3] << std::endl;

    return ResultStatus::Good();
}

template <uint64_t size>
bool CommandVectorInput<size>::sendToServer(MyVec &vec) const {
    if (!ipAddress.checkIpInput()) {
        logger(RES_WARNING("Input ip (select this command in menu)."));
        return false;
    }

    SocketClient client(ipAddress.getIp(), ipAddress.getPort());

    if (!client.clientIsRunning()) {
        return false;
    }
    ResultStatus status;

    switch (hashString(vec.getType().c_str())) {
        case hashString("int"):
            status = processIntVec(vec.getIntVec(), client);
            break;

        case hashString("float"):
            status = processFloatVec(vec.getFloatVec(), client);
            break;

        case hashString("string"):
            status = processStringVec(vec.getStringVec(), client);
            break;

        default:
            status = RES_ERROR("Type error.");
    }

    logger(status);

    if (status.isError()) {
        return false;
    }

    return true;
}

template <uint64_t size>
void CommandVectorInput<size>::action() {

    std::vector<std::string> tempStringVec;
    std::vector<int64_t> tempIntVec;
    std::vector<float> tempFloatVec;

    const ResultStatus res = processInputVector(type, tempStringVec,
        tempIntVec, tempFloatVec, vecSize);

    logger(res);

    if (res.isError()) {
        std::cout << "Press any key to continue: ";
        getchar();
        return;
    }

    MyVec vec;

    switch (hashString(type.c_str())) {
        case hashString("int"):
            vec = MyVec(std::move(tempIntVec));
            break;
        case hashString("float"):
            vec = MyVec(std::move(tempFloatVec));
            break;
        case hashString("string"):
            vec = MyVec(std::move(tempStringVec));
            break;
        default:
            return;
    }

    if (!pool.insert(vec)) {
        std::cout << "Press any key to continue: ";
        getchar();
        return;
    }

    sendToServer(vec);
    std::cout << "Press any key to continue: ";
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
        logger(RES_WARNING("No one vector input."));
    }else {
        MyVec vec = pool.last();
        switch (hashString(vec.getType().c_str())) {
            case(hashString("int")):
                printVector(vec.getIntVec(), size, vec.getType());
                break;

            case(hashString("float")):
                printVector(vec.getFloatVec(), size, vec.getType());
                break;

            case(hashString("string")):
                printVector(vec.getStringVec(), size, vec.getType());
                break;

            default:;
        }
    }

    std::cout << "Your current type: " << type << std::endl;
    std::cout << "Your current ip: " << ipAddress << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << "Press any key to continue: ";
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
