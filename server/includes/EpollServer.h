//
// Created by fisp on 15.03.2026.
//

#pragma once

#include <sys/epoll.h>
#include <memory>
#include "../../commonFunc/includes/ResultStatus.h"

// Инвариант - epoll_fd == -1 epoll отсутствует
// - epoll_fd >= 0 создан
// - event_client список прослушиваемых клиентов
// - event_server настройка для сервера
class EpollServer {
    int32_t epoll_fd = -1;
    epoll_event event_server{};
    std::unique_ptr<epoll_event[]> event_client;

    ResultStatus createEpollFd();
    ResultStatus settingEpollFd(int32_t server_fd);
public:
    explicit EpollServer(int32_t server_fd, uint32_t sizeClients);
    ~EpollServer();
    int32_t waitEpollFd(uint32_t size, ResultStatus &res) const;
    ResultStatus closeEpollFd();
    ResultStatus addEpollFd(int32_t clientSocket) const;
    int32_t getEventClient( uint32_t index) const;

};

