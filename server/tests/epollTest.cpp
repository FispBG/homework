//
// Created by fisp on 26.03.2026.
//

#include <gtest/gtest.h>

#include "../includes/SocketServer.h"
#include "../../commonFunc/includes/ResultStatus.h"
#include "../includes/EpollServer.h"


TEST(EpollServerTest, CloseEpollTwice) {
    const SocketServer server(12348, 1);
    EpollServer epoll(server.getSocket(), 10);

    const ResultStatus res1 = epoll.closeEpollFd();
    const ResultStatus res2 = epoll.closeEpollFd();

    EXPECT_TRUE(res2.isError());
}