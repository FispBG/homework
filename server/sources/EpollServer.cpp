//
// Created by fisp on 15.03.2026.
//

#include "../includes/EpollServer.h"

#include <unistd.h>

EpollServer::EpollServer(const int32_t server_fd, const uint32_t sizeClients) {
    ResultStatus status = createEpollFd();
    logger(status);

    status = settingEpollFd(server_fd);
    logger(status);

    event_client = std::make_unique<epoll_event[]>(sizeClients);
}

int32_t EpollServer::getEventClient(const uint32_t index) const {
    return event_client[index].data.fd;
}

ResultStatus EpollServer::createEpollFd() {
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        return RES_ERROR("Epoll server dont create.");
    }

    return ResultStatus::Good();
}

ResultStatus EpollServer::settingEpollFd(const int32_t server_fd) {
    event_server.data.fd = server_fd;
    event_server.events = EPOLLIN;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event_server) < 0) {
        return RES_ERROR("Epoll server dont add epoll fd.");
    }

    return ResultStatus::Good();
}

EpollServer::~EpollServer() {
    close(epoll_fd);
}

ResultStatus EpollServer::addEpollFd(const int32_t clientSocket) const {
    if (epoll_fd < 0) {
        return RES_ERROR("Epoll server dont create epoll fd.");
    }

    epoll_event event{};
    event.data.fd = clientSocket;
    event.events = EPOLLIN | EPOLLONESHOT;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientSocket, &event) < 0) {
        return RES_ERROR("Epoll server dont add epoll fd.");
    }

    return ResultStatus::Good();
}


ResultStatus EpollServer::closeEpollFd() {
    if (epoll_fd < 0) {
        return RES_ERROR("Epoll server dont create.");
    }
    if (close(epoll_fd) < 0) {
        return RES_ERROR("Epoll server dont close epoll fd.");
    }

    epoll_fd = -1;

    return ResultStatus::Good();
}

int32_t EpollServer::waitEpollFd(const uint32_t size, ResultStatus &res) const {
    if (epoll_fd < 0) {
        res = RES_ERROR("Epoll server dont create.");
        return -1;
    }

    const int32_t count_user = epoll_wait(epoll_fd, event_client.get(), size, -1);

    if (count_user < 0) {
        res = RES_ERROR("Epoll server dont wait epoll event.");
        return -1;
    }

    res = ResultStatus::Good();
    return count_user;
}
