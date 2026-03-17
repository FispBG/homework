#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "../includes/SocketServer.h"
#include "../includes/EpollServer.h"
#include "../includes/ManageServer.h"

#define PORT 8080
#define COUNT_USER 5

int main(){
    auto server = SocketServer(PORT, COUNT_USER);

    auto epoll_server = EpollServer(server.getSocket(), COUNT_USER + 1);

    auto manager = ManageServer(server, epoll_server);

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
