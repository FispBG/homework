//
// Created by fisp on 15.03.2026.
//

#include "../includes/ManageServer.h"
#include "../includes/MathFunc.h"
#include <unistd.h>

void ManageServer::CheckUpdateSocket(const int32_t countSocketUpdate) {
    for (uint32_t i = 0; i < countSocketUpdate; i++) {
        const int32_t socketUpdate = epoll_fd.getEventClient(i);

        if (socketUpdate == socket_fd.getSocket()) {
            addNewClientSocket();

        }else {
            thread_pool.addNewTask([this, socketUpdate] {
                processClientRequest(socketUpdate);
            });
        }
    }
}

void ManageServer::addNewClientSocket() const {
    ResultStatus status;
    const int32_t clientSocket = socket_fd.acceptSocket(status);
    if (status.isError()) {
        logger(status);
        return;
    }

    status = epoll_fd.addEpollFd(clientSocket);

    if (status.isError()) {
        logger(status);
        return;
    }

}

auto toFloat = [](const uint32_t networkVal) {
    const uint32_t hostVal = ntohl(networkVal);
    float result;
    std::memcpy(&result, &hostVal, sizeof(float));
    return result;
};

auto toInt = [](const uint32_t networkVal) {
    const uint32_t hostVal = ntohl(networkVal);
    int32_t result;
    std::memcpy(&result, &hostVal, sizeof(int32_t));
    return result;
};

void ManageServer::processClientRequest(const int32_t clientSocket)  {
    std::vector<std::byte> packet;
    ResultStatus status = socket_fd.receiveMessage(clientSocket, packet);

    if (status.isError()) {
        logger(status);
        close(clientSocket);
        return;
    }

    if (packet.size() == 0) {
       logger(RES_ERROR("Accept empty packet."));
        return;
    }

    const auto packetType = static_cast<PacketType>(packet[0]);


    if (packetType == PacketType::VEC_FLOAT) {

        const Vec4 answer = processVec(packet, toFloat);
        sendVector(clientSocket, answer);

    }else if (packetType == PacketType::VEC_INT) {

        const Vec4 answer = processVec(packet, toInt);
        sendVector(clientSocket, answer);

    }else if (packetType == PacketType::STRING) {

        const std::string text = processString(packet);
        const Vec4 answer = textToIntVec(text);
        sendVector(clientSocket, answer);
    }
}

std::string ManageServer::processString(const std::vector<std::byte> &packet) {
    constexpr int32_t sizeHeader = sizeof(HeaderString);

    if (packet.size() < sizeHeader) {
        logger(RES_ERROR("Accept broken packet."));
        return "";
    }

    HeaderString header;
    std::memcpy(&header, packet.data(), sizeof(header));

    const int32_t sizeString = ntohl(header.size);
    if (packet.size() < (sizeHeader + sizeString)) {
        logger(RES_ERROR("Text accepted not full."));
        return "";
    }

    const std::string text(reinterpret_cast<const char*>(packet.data() + sizeHeader), sizeString);

    return text;
}

void ManageServer::sendVector(const int32_t clientSocket, const Vec4 &vec) const {

    PacketVector packet = createPacketVector(vec.x, vec.y, vec.z, vec.w);

    const ResultStatus res =  socket_fd.sendMessage(clientSocket,
        reinterpret_cast<const char*> (&packet), sizeof(packet));

    if (res.isError()) {
        logger(res);
    }
}

void ManageServer::sendInteger(const int32_t clientSocket, const int32_t integer) const {

    const int32_t intToNetwork = htonl(static_cast<uint32_t>(integer));
    const ResultStatus res =  socket_fd.sendMessage(clientSocket,
        reinterpret_cast<const char*> (&intToNetwork), sizeof(intToNetwork));

    if (res.isError()) {
        logger(res);
    }
}