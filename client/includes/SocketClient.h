//
// Created by fisp on 16.03.2026.
//

#pragma once

#include <unistd.h>
#include <sys/socket.h>

#include "../../commonFunc/includes/functions.h"
#include "../../commonFunc/includes/StandardPackets.h"

// Инвариант - clientSocket == -1 нет соединений
// clientSocket >= 0 - соединение активно
class SocketClient {
    int32_t clientSocket = -1;

    ResultStatus createSocketClient();
    ResultStatus settingSocket(const std::string &ip, uint16_t port);
public:
    SocketClient(const std::string &ip, uint16_t port);
    ~SocketClient();

    template <typename T>
    bool sendPacket(const T &packet);
    bool sendSize(const uint32_t &size);

    template <typename T>
    bool readSocket(T &packet, int32_t socket);


    template <typename T>
    std::vector<T> receiveVector();

    int32_t receiveSize();

    bool clientIsRunning() const;
};

template <typename T>
bool SocketClient::sendPacket(const T &packet) {
    if (clientSocket < 0) {
        return false;
    }

    constexpr int32_t bytesTarget = sizeof(packet);

    if (!sendSize(bytesTarget)) {
        logger(RES_ERROR("failed send size packet"));
        return false;
    }

    int32_t bytesSent = 0;
    const auto ptr = reinterpret_cast<const uint8_t *>(&packet);

    while (bytesSent < bytesTarget) {
        const int32_t bytes = send(clientSocket, ptr + bytesSent, bytesTarget - bytesSent, 0);

        if (bytes < 0) {
            close(clientSocket);
            clientSocket = -1;
            return false;
        }

        bytesSent += bytes;
    }

    return true;
}

template <typename T>
bool SocketClient::readSocket(T &packet, const int32_t socket) {
    if (clientSocket < 0) {
        return false;
    }

    int32_t bytesReceived = 0;
    constexpr int32_t bytesTarget = sizeof(packet);

    const auto prt = reinterpret_cast<uint8_t *>(&packet);
    while (bytesReceived < bytesTarget) {
        const int32_t bytes = read(socket, prt + bytesReceived, bytesTarget - bytesReceived);
        if (bytes <= 0) {
            return false;
        }
        bytesReceived += bytes;
    }

    return true;
}

template <typename transform, typename T>
void transformValueToVec4(const PacketVector &packet, std::vector<T> &answer, transform transformFunc) {
    answer[0] = transformFunc(packet.data.x);
    answer[1] = transformFunc(packet.data.y);
    answer[2] = transformFunc(packet.data.z);
    answer[3] = transformFunc(packet.data.w);
}

template <typename T>
std::vector<T> SocketClient::receiveVector() {
    PacketVector packetVector{};
    std::vector<T> answer(4);

    if (receiveSize() < 0) {
        logger(RES_ERROR("Server dont answer."));
        return answer;
    }

    if (!readSocket(packetVector, clientSocket)) {
        logger(RES_ERROR("Server dont answer."));
        return answer;
    }


    if (packetVector.type == PacketType::VEC_FLOAT) {
        auto takeFloat = [](const uint32_t value) {
            static_assert(sizeof(float) == sizeof(uint32_t), "size float != uint32_t");
            const uint32_t hostValue = ntohl(value);
            float resultValue;
            std::memcpy(&resultValue, &hostValue, sizeof(float));
            return resultValue;
        };

        transformValueToVec4(packetVector, answer, takeFloat);
    }else if (packetVector.type == PacketType::VEC_INT) {
        auto takeInt = [](const uint32_t value) {
            const uint32_t hostValue = ntohl(value);
            int32_t resultValue;
            std::memcpy(&resultValue, &hostValue, sizeof(int32_t));
            return resultValue;
        };
        transformValueToVec4(packetVector, answer, takeInt);
    }

    return answer;
}