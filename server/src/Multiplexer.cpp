#include <iostream>
#include <fcntl.h>
#include "./include/Multiplexer.hpp"
#include <unistd.h>

using namespace Multiplexing;
using namespace SocketServer;

void Multiplexer::init() {
    this->efd = epoll_create1(0);
    if(this->efd == -1) {
        perror("epoll_create1");
        exit(1);
    }
}
void Multiplexer::loopEvent(CommandQueue* cmd) {
    struct epoll_event event, events[this->maxEvents];
    event.events = EPOLLIN;
    event.data.fd = this->s->fd;
    epoll_ctl(this->efd, EPOLL_CTL_ADD, this->s->fd, &event);    
    while(1) {
        int countReady = epoll_wait(this->efd, events, this->maxEvents, -1);

        for(int i = 0; i < countReady; i++) {
            if(events[i].data.fd == this->s->fd) {
                int cfd = this->s->acceptConnection();
                this->s->setnonblock(cfd);
                event.events = EPOLLIN;
                event.data.fd = cfd;
                epoll_ctl(this->efd, EPOLL_CTL_ADD, cfd, &event);
            } else {
                if(events[i].events & EPOLLIN) {
                    char buf[20] = {};
                    int rc = read(events[i].data.fd, buf, 20);
                    if(rc <= 0) {
                        close(events[i].data.fd);
                        epoll_ctl(this->efd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                    } else {    
                        Command cd;
                        cd.clientFd = events[i].data.fd;
                        cd.cmd = buf;
                        cmd->add(cd);
                    }  
                }
                if(events[i].events & EPOLLOUT)
                    ;//write

            }
        }
    }
}
