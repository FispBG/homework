//
// Created by Fisp on 13.03.2026.
//

#pragma once

#include <vector>
#include <netinet/in.h>
#include <string>
#include "../../commonFunc/includes/ResultStatus.h"

// Инвариант - socket_my == -1 сервер не создан
// - socket_my >= 0 создан
// - addr хранит настройки сервера
class SocketServer {
    int32_t socket_my = -1;
    sockaddr_in addr{};

    ResultStatus createServer();
    ResultStatus settingServer(uint16_t port);
    ResultStatus startServer(uint16_t countClient) const;
public:
    SocketServer(uint16_t port, uint16_t countClient);
    ~SocketServer();
    int32_t acceptSocket(ResultStatus &status);

    ResultStatus setSocketNonBlock(int32_t clientSocket) const;

    ResultStatus receiveMessage(int32_t clientSocket, std::vector<std::byte> &message);
    ResultStatus sendMessage(int32_t clientSocket, const char *message, uint32_t size) const;
    ResultStatus closeSocket();
    int32_t getSocket() const;
};