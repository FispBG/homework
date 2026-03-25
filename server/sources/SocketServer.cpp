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
        return RES_ERROR("Socket creation failed.");
    }

    return ResultStatus::Good();
}

ResultStatus SocketServer::settingServer(const uint16_t port) {
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(socket_my,reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        return RES_ERROR("Bind failed.");
    }

    return ResultStatus::Good();
}

ResultStatus SocketServer::startServer(const uint16_t countClient) const {
    if (socket_my < 0) {
        return RES_ERROR("Socket not created.");
    }

    if (listen(socket_my, countClient) < 0) {
        return RES_ERROR("Listen failed.");
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
        status = RES_ERROR("Socket not created.");
        return -1;
    }

    socklen_t addrLength = sizeof(addr);
    const int32_t clientSocket = accept(socket_my, reinterpret_cast<sockaddr *>(&addr),
        &addrLength);

    if (clientSocket < 0) {
        status = RES_ERROR("Accept failed.");
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
        return RES_ERROR("Failed getting socket flag.");
    }

    if (fcntl(clientSocket, F_SETFL, flag | O_NONBLOCK) < 0) {
        return RES_ERROR("Failed setting socket flag.");
    }

    return ResultStatus::Good();
}


ResultStatus SocketServer::receiveMessage(const int32_t clientSocket, std::vector<std::byte> &message) {
    if (clientSocket < 0) {
        return RES_ERROR("Socket client not created.");
    }

    uint32_t bytesWait = 0;
    const int32_t readWaitBytes = read(clientSocket, &bytesWait, sizeof(bytesWait));

    if (readWaitBytes <= 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            message.clear();
            return ResultStatus::Good();
        }
        return RES_ERROR("Failed to read size client socket.");
    }

    bytesWait = ntohl(bytesWait);
    std::cout << "bytesWait: " << bytesWait << std::endl;
    std::vector<std::byte> messageRead;
    messageRead.resize(bytesWait);

    uint32_t bytesRead = 0;

    while (bytesRead < bytesWait) {

        const int32_t bytes = read(clientSocket, messageRead.data() + bytesRead, bytesWait - bytesRead);
        std::cout << "bytes: " << bytes << std::endl;
        if (bytes < 0) {
            return RES_ERROR("Failed to read from client socket.");
        }
        if (bytes == 0) {
            return RES_ERROR("Client closed.");
        }

        bytesRead += bytes;
    }

    message = std::move(messageRead);
    return ResultStatus::Good();
}

ResultStatus SocketServer::sendMessage(const int32_t clientSocket, const char *message, const uint32_t size) const {
    if (clientSocket < 0) {
        return RES_ERROR("Socket not created.");
    }

    const uint32_t sizeToSend = htonl(size);
    const int32_t sendSize = send(clientSocket, &sizeToSend, sizeof(sizeToSend), 0);

    if (sendSize < 0) {
        return RES_ERROR("Send failed.");
    }

    int32_t bytesSent = 0;

    while (bytesSent < size) {
        const int32_t bytes = send(clientSocket, message + bytesSent, size - bytesSent, 0);

        if (bytes <= 0) {
            return RES_ERROR("Send failed.");
        }

        bytesSent += bytes;
    }

    return ResultStatus::Good();
}

ResultStatus SocketServer::closeSocket() {
    if (socket_my < 0) {
        return RES_ERROR("Socket not created.");
    }
    if (close(socket_my) < 0) {
        return RES_ERROR("Close failed.");
    }
    socket_my = -1;

    return ResultStatus::Good();
}