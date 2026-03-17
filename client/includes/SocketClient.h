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

    template <typename T>
    bool readSocket(T &packet, int32_t socket);


    Vec4 receiveVectorFloat();
    int32_t receiveInt();

    bool clientIsRunning() const;
};

template <typename T>
bool SocketClient::sendPacket(const T &packet) {
    if (clientSocket < 0) {
        return false;
    }

    int32_t bytesSent = 0;
    constexpr int32_t bytesTarget = sizeof(packet);

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
