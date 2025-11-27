#include <iostream>
#include "./include/Multiplexer.hpp"

using namespace Multiplexing;
using namespace SocketServer;

void Multiplexer::init() {
    this->efd = epoll_create1(0);
    if(this->efd == -1) {
        perror("epoll_create1");
        exit(1);
    }
}
void Multiplexer::loopEvent() {
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.u32 = this->s->fd;
    epoll_ctl(this->efd, EPOLL_CTL_ADD, this->s->fd, &event);    
    struct epoll_event events[this->maxEvents];
    while(1) {
        int countReady = epoll_wait(this->efd, events, this->maxEvents, -1);

        for(int i = 0; i < countReady; i++) {
            if(events[i].data.u32 == this->s->fd) {
                int cfd = this->s->acceptConnection();
                event.events = EPOLLIN;
                event.data.u32 = cfd;
                epoll_ctl(this->efd, EPOLL_CTL_ADD, cfd, &event);
            } else {
                if(events[i].event & EPOLLIN)
                    ;//read
                if(events[i].event & EPOLLOUT)i
                    ;//write

            }
        }
    }
}
