//
// Created by fisp on 16.03.2026.
//

#include "../includes/SocketClient.h"

#include <cstring>
#include <arpa/inet.h>

bool SocketClient::clientIsRunning() const {
    if (clientSocket == -1) {
        return false;
    }

    return true;
}

Vec4 SocketClient::receiveVectorFloat() {
    PacketVector packetVector{};
    Vec4 answer {0, 0, 0, 0};

    if (!readSocket(packetVector, clientSocket)) {
        logger(ResultStatus::Error("Server dont answer."));
        return answer;
    }

    if (packetVector.type == PacketType::VEC_FLOAT) {
        auto takeFloat = [](const uint32_t value) {
            const uint32_t hostValue = ntohl(value);
            float resultValue;
            std::memcpy(&resultValue, &hostValue, sizeof(float));
            return resultValue;
        };

        answer.x = takeFloat(packetVector.data.x);
        answer.y = takeFloat(packetVector.data.y);
        answer.z = takeFloat(packetVector.data.z);
        answer.w = takeFloat(packetVector.data.w);
    }

    return answer;
}

int32_t SocketClient::receiveInt() {
    uint32_t answer = 0;

    if (!readSocket(answer, clientSocket)) {
        logger(ResultStatus::Error("Server dont answer."));
        return -1;
    }

    return ntohl(answer);
}

ResultStatus SocketClient::createSocketClient() {
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket < 0) {
        return ResultStatus::Error("Cannot create socket.");
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
        return ResultStatus::Error("Cannot connect to server.");
    }

    constexpr timeval timeout{5, 0};
    if (setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        close(clientSocket);
        clientSocket = -1;
        return ResultStatus::Error("Cannot set socket timeout.");
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