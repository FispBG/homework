//
// Created by fisp on 16.03.2026.
//

#include "../includes/SocketClient.h"

#include <arpa/inet.h>

bool SocketClient::clientIsRunning() const {
    if (clientSocket == -1) {
        return false;
    }

    return true;
}

int32_t SocketClient::receiveSize() {
    uint32_t answer = 0;

    if (!readSocket(answer, clientSocket)) {
        logger(RES_ERROR("Server dont answer."));
        return -1;
    }

    return ntohl(answer);
}

ResultStatus SocketClient::createSocketClient() {
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket < 0) {
        return RES_ERROR("Cannot create socket.");
    }

    return ResultStatus::Good();
}

ResultStatus SocketClient::settingSocket(const std::string &ip, const uint16_t port) {

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    if (connect(clientSocket, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) < 0) {
        close(clientSocket);
        clientSocket = -1;
        return RES_ERROR("Cannot connect to server.");
    }

    constexpr timeval timeout{5, 0};
    if (setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        close(clientSocket);
        clientSocket = -1;
        return RES_ERROR("Cannot set socket timeout.");
    }

    return ResultStatus::Good();
}

SocketClient::SocketClient(const std::string &ip, const uint16_t port) {
    ResultStatus status = createSocketClient();
    logger(status);

    if (clientSocket < 0) {
        return;
    }

    status = settingSocket(ip, port);
    logger(status);

    if (clientSocket != -1) {
        std::cout << "Connect good." << std::endl;
    }
}

SocketClient::~SocketClient() {
    if (clientSocket != -1) {
        close(clientSocket);
    }
}

bool SocketClient::sendSize(const uint32_t &size) {
    if (clientSocket <= -1) {
        return false;
    }

    const uint32_t networkValue = htonl(size);
    const int32_t bytes = send(clientSocket, &networkValue, sizeof(networkValue), 0);

    if (bytes < 0) {
        close(clientSocket);
        clientSocket = -1;
        return false;
    }

    return true;
}
