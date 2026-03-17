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
            processClientRequest(socketUpdate);
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

void ManageServer::processClientRequest(const int32_t clientSocket)  {
    std::vector<char> packet;
    ResultStatus status = socket_fd.receiveMessage(clientSocket, packet);

    if (status.isError()) {
        logger(status);
        close(clientSocket);
        return;
    }

    if (packet.size() == 0) {
       logger(ResultStatus::Error("Accept empty packet."));
        return;
    }

    const auto packetType = static_cast<PacketType>(packet[0]);

    if (packetType == PacketType::VEC_FLOAT) {

        const Vec4 answer = processVec(packet);
        sendVectorFloat(clientSocket, answer);

    }else if (packetType == PacketType::STRING) {

        const int32_t sizeText = processString(packet);
        sendInteger(clientSocket, sizeText);
    }
}

Vec4 ManageServer::processVec(const std::vector<char> &packet) {
    if (packet.size() < sizeof(PacketVector)) {
        logger(ResultStatus::Error("Accept broken packet."));
        return Vec4();
    }

    PacketVector packetVector{};
    std::memcpy(&packetVector, packet.data(), sizeof(PacketVector));

    auto toFloat = [](const uint32_t networkVal) {
        const uint32_t hostVal = ntohl(networkVal);
        float result;
        std::memcpy(&result, &hostVal, sizeof(float));
        return result;
    };

    Vec4 fromNetworkVec{};

    fromNetworkVec.x = toFloat(packetVector.data.x);
    fromNetworkVec.y = toFloat(packetVector.data.y);
    fromNetworkVec.z = toFloat(packetVector.data.z);
    fromNetworkVec.w = toFloat(packetVector.data.w);

    const Vec4 answer = multiOnMatrix(fromNetworkVec);

    return answer;
}

int32_t ManageServer::processString(const std::vector<char> &packet) {
    constexpr int32_t sizeHeader = sizeof(HeaderString);

    if (packet.size() < sizeHeader) {
        logger(ResultStatus::Error("Accept broken packet."));
        return 0;
    }

    const auto header = reinterpret_cast<const HeaderString &>(*packet.data());
    const int32_t sizeString = ntohl(header.size);
    if (packet.size() < (sizeHeader + sizeString)) {
        logger(ResultStatus::Error("Text accepted not full."));
        return 0;
    }

    const std::string text(packet.begin() + sizeHeader,
        packet.begin() + sizeHeader + sizeString);

    return text.length();
}

void ManageServer::sendVectorFloat(const int32_t clientSocket, const Vec4 &vec) const {

    PacketVector packet = createPacketVector(vec.x, vec.y, vec.z, vec.w);

    const ResultStatus res =  socket_fd.sendMessage(clientSocket,
        reinterpret_cast<char*> (&packet), sizeof(packet));

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
