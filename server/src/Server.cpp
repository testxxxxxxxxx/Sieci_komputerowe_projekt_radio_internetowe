#include <iostream>
#include "./include/Server.hpp"
#include <fcntl.h>

using namespace std;
using namespace SocketServer;

void Server::init() {
    this->fd = socket(AF_INET, SOCK_STREAM, 0);

    if(this->fd < 0) {
        perror("Socket error");
        exit(1);
    }

    int opt = 1;

    this->saddr.sin_family = AF_INET;
    this->saddr.sin_addr.s_addr = INADDR_ANY;
    this->saddr.sin_port = htons(this->port);

    if(setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Setsockopt error");
        exit(1);
    }

    if(bind(this->fd, (struct sockaddr*)& this->saddr, sizeof(this->saddr)) < 0) {
        perror("Bind error");
        exit(1);
    }

    if(listen(this->fd, 10) < 0) {
        perror("listen error");
        exit(1);
    }

    int flags = fcntl(this->fd, F_GETFL, 0);
    fcntl(this->fd, F_SETFL, flags | O_NONBLOCK);
}
int Server::acceptConnection() {
    socklen_t caddrLen = sizeof(this->caddr);
    int cfd = accept(this->fd, (struct sockaddr*)& this->caddr, &caddrLen);
    if(cfd < 0) {
        if(errno == -1 || errno == EWOULDBLOCK) 
            return -1;
        perror("accept error");
        return -1;
    }
    return cfd;
}
