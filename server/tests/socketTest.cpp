//
// Created by fisp on 28.02.2026.
//

#include <gtest/gtest.h>

#include "../includes/SocketServer.h"
#include "../../commonFunc/includes/ResultStatus.h"

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
