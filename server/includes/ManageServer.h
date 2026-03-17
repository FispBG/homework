//
// Created by fisp on 15.03.2026.
//

#pragma once

#include "../includes/EpollServer.h"
#include "../includes/SocketServer.h"
#include "../../commonFunc/includes/StandardPackets.h"


// Инвариант - socket_fd >= 0
// - epoll_fd >= 0
class ManageServer {
    SocketServer &socket_fd;
    EpollServer &epoll_fd;

    Vec4 processVec(const std::vector<char> &packet);
    int32_t processString(const std::vector<char> &packet);
public:
    ManageServer(SocketServer &server_fd_in, EpollServer &epoll_fd_in) :
        socket_fd(server_fd_in), epoll_fd(epoll_fd_in) {};

    void CheckUpdateSocket(int32_t countSocketUpdate);
    void addNewClientSocket() const;
    void processClientRequest(int32_t clientSocket);
    void sendVectorFloat(int32_t clientSocket, const Vec4 &vec) const;
    void sendInteger(int32_t clientSocket, int32_t integer) const;
};
