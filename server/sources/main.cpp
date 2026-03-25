#include <iostream>
#include "../includes/SocketServer.h"
#include "../includes/EpollServer.h"
#include "../includes/ManageServer.h"

constexpr size_t PORT = 8080;
constexpr size_t COUNT_USER = 4;

int main(){
    auto server = SocketServer(PORT, COUNT_USER);

    auto epoll_server = EpollServer(server.getSocket(), COUNT_USER + 1);

    auto manager = ManageServer(server, epoll_server, COUNT_USER);

    std::cout << "Server started" << std::endl;

    while(true) {
        ResultStatus status;
        const int32_t countSocketsUpdate = epoll_server.waitEpollFd(COUNT_USER, status);

        if (status.isError()) {
            continue;
        }

        manager.CheckUpdateSocket(countSocketsUpdate);
    }


    return 0;
}
