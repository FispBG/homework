//
// Created by fisp on 15.03.2026.
//

#pragma once

#include "MathFunc.h"
#include "../includes/EpollServer.h"
#include "../includes/SocketServer.h"
#include "../../commonFunc/includes/StandardPackets.h"
#include "../includes/ThreadPool.h"


// Инвариант - socket_fd >= 0
// - epoll_fd >= 0
class ManageServer {
    SocketServer &socket_fd;
    EpollServer &epoll_fd;
    ThreadPool thread_pool;

    template <typename transform>
    Vec4 processVec(const std::vector<std::byte> &packet, const transform &trans);
    std::string processString(const std::vector<std::byte> &packet);
public:
    ManageServer(SocketServer &server_fd_in, EpollServer &epoll_fd_in, const uint32_t size) :
        socket_fd(server_fd_in), epoll_fd(epoll_fd_in), thread_pool(size) {};

    void CheckUpdateSocket(int32_t countSocketUpdate);
    void addNewClientSocket() const;
    void processClientRequest(int32_t clientSocket);
    void sendVector(int32_t clientSocket, const Vec4 &vec) const;
    void sendInteger(int32_t clientSocket, int32_t integer) const;
};

template <typename transform>
Vec4 ManageServer::processVec(const std::vector<std::byte> &packet, const transform &trans) {
    if (packet.size() < sizeof(PacketVector)) {
        logger(RES_ERROR("Accept broken packet."));
        return Vec4{};
    }

    PacketVector packetVector{};
    std::memcpy(&packetVector, packet.data(), sizeof(PacketVector));

    const Vec4 answer = multiOnMatrix(trans(packetVector.data.x),
        trans(packetVector.data.y),
        trans(packetVector.data.z),
        trans(packetVector.data.w));

    return answer;
}