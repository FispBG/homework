//
// Created by fisp on 28.02.2026.
//

#include <gtest/gtest.h>

#include "../includes/MathFunc.h"
#include "../includes/SocketServer.h"
#include "../includes/EpollServer.h"
#include "../../commonFunc/includes/functions.h"
#include "../../commonFunc/includes/StandardPackets.h"

// mathfunc.h

TEST(MultiOnMatrixTest, CheckMultiOnMatrix) {
    const Vec4 input = {1.0f, 2.0f, 3.0f, 4.0f};
    const Vec4 result = multiOnMatrix(input);

    const Vec4 zero  = {0.0f, 0.0f, 0.0f, 0.0f};
    EXPECT_FALSE(result == zero);
}

// socketserver.h

TEST(SocketServerTest, InvalidPort) {
    const SocketServer server(0, 1);

    EXPECT_GE(server.getSocket(), 0);
}

TEST(SocketServerTest, ValidPortCreate) {
    const SocketServer server(12345, 1);

    EXPECT_GE(server.getSocket(), 0);
}

TEST(SocketServerTest, CloseSocketTwice) {
    SocketServer server(12346, 1);

    const ResultStatus res1 = server.closeSocket();
    const ResultStatus res2 = server.closeSocket();

    EXPECT_TRUE(res2.isError());
}

TEST(SocketServerTest, AcceptOnCloseSocket) {
    SocketServer server(12347, 1);
    server.closeSocket();

    ResultStatus status = ResultStatus::Good();
    const int32_t clientFd = server.acceptSocket(status);

    EXPECT_LT(clientFd, 0);
    EXPECT_TRUE(status.isError());
}

// epollserver.h

TEST(EpollServerTest, WaitCloseSocket) {
    const EpollServer epoll(-1, 10);

    ResultStatus res = ResultStatus::Good();
    const int32_t count = epoll.waitEpollFd(10, res);

    EXPECT_LT(count, 0);
    EXPECT_TRUE(res.isError());
}

TEST(EpollServerTest, CloseEpollTwice) {
    const SocketServer server(12348, 1);
    EpollServer epoll(server.getSocket(), 10);

    const ResultStatus res1 = epoll.closeEpollFd();
    const ResultStatus res2 = epoll.closeEpollFd();

    EXPECT_TRUE(res2.isError());
}