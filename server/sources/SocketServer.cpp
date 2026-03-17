//
// Created by Fisp on 13.03.2026.
//

#include "../includes/SocketServer.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

ResultStatus SocketServer::createServer() {
    socket_my  = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (socket_my < 0) {
        return ResultStatus::Error("Socket creation failed.");
    }

    return ResultStatus::Good();
}

ResultStatus SocketServer::settingServer(const uint16_t port) {
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(socket_my,reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        return ResultStatus::Error("Bind failed.");
    }

    return ResultStatus::Good();
}

ResultStatus SocketServer::startServer(const uint16_t countClient) const {
    if (socket_my < 0) {
        return ResultStatus::Error("Socket not created.");
    }

    if (listen(socket_my, countClient) < 0) {
        return ResultStatus::Error("Listen failed.");
    }

    return ResultStatus::Good();
}

SocketServer::SocketServer(const uint16_t port, const uint16_t countClient) {
    ResultStatus status = createServer();
    logger(status);
    if (status.isError()) {
        return;
    }

    status = settingServer(port);
    logger(status);
    if (status.isError()) {
        return;
    }

    status = startServer(countClient);
    logger(status);
}

SocketServer::~SocketServer() {
    close(socket_my);
}

int32_t SocketServer::getSocket() const {
    return socket_my;
}

int32_t SocketServer::acceptSocket(ResultStatus &status) {
    if (socket_my < 0) {
        status = ResultStatus::Error("Socket not created.");
        return -1;
    }

    socklen_t addrLength = sizeof(addr);
    const int32_t clientSocket = accept(socket_my, reinterpret_cast<sockaddr *>(&addr),
        &addrLength);

    if (clientSocket < 0) {
        status = ResultStatus::Error("Accept failed.");
        return -1;
    }

    status = setSocketNonBlock(clientSocket);
    if (status.isError()) {
        close(clientSocket);
        return -1;
    }

    return clientSocket;
}

ResultStatus SocketServer::setSocketNonBlock(int32_t clientSocket) const {
    const int flag = fcntl(clientSocket, F_GETFL);

    if (flag < 0) {
        return ResultStatus::Error("Failed getting socket flag.");
    }

    if (fcntl(clientSocket, F_SETFL, flag | O_NONBLOCK) < 0) {
        return ResultStatus::Error("Failed setting socket flag.");
    }

    return ResultStatus::Good();
}


ResultStatus SocketServer::receiveMessage(const int32_t clientSocket, std::vector<char> &message) {
    if (clientSocket < 0) {
        return ResultStatus::Error("Socket client not created.");
    }

    std::vector<char> messageRead(1024);

    const int32_t bytesRead = read(clientSocket, messageRead.data(), messageRead.size());

    if (bytesRead < 0) {
        return ResultStatus::Error("Read failed.");
    }

    if (bytesRead == 0) {
        return ResultStatus::Error("Client closed.");
    }

    messageRead.resize(bytesRead);

    message = std::move(messageRead);
    return ResultStatus::Good();
}

ResultStatus SocketServer::sendMessage(const int32_t clientSocket, const char *message, const uint32_t size) const {
    if (clientSocket < 0) {
        return ResultStatus::Error("Socket not created.");
    }

    int32_t bytesSent = 0;

    while (bytesSent < size) {
        const int32_t bytes = send(clientSocket, message + bytesSent, size - bytesSent, 0);

        if (bytes < 0) {
            return ResultStatus::Error("Send failed.");
        }

        bytesSent += bytes;
    }

    return ResultStatus::Good();
}

ResultStatus SocketServer::closeSocket() {
    if (socket_my < 0) {
        return ResultStatus::Error("Socket not created.");
    }
    if (close(socket_my) < 0) {
        return ResultStatus::Error("Close failed.");
    }
    socket_my = -1;

    return ResultStatus::Good();
}
