#include <iostream>
#include <fcntl.h>
#include "../include/Multiplexer.hpp"
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
                this->clients[cfd] = new Client(cfd);
            } else {
                if(events[i].events & EPOLLIN) {
                    auto &cl = this->clients[events[i].data.fd];
                    char buf[256] = {};
                    int rc = read(events[i].data.fd, buf, 256);
                    if(rc <= 0) {
                        if(cl->mode == Client::UPLOAD && cl->uploadFd != -1)
                            close(cl->uploadFd);
                        close(events[i].data.fd);
                        delete cl;
                        epoll_ctl(this->efd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                        this->clients.erase(events[i].data.fd);
                        continue;
                    }
                    if (cl->mode == Client::UPLOAD) {
                        long long toWrite = min<long long>(rc, cl->uploadBytesLeft);
                        write(cl->uploadFd, buf, toWrite);
                        cl->uploadBytesLeft -= toWrite;

                        if (rc > toWrite)
                            cl->recvBuffer.append(buf + toWrite, rc - toWrite);

                        if (cl->uploadBytesLeft <= 0) {
                            close(cl->uploadFd);
                            cl->uploadFd = -1;
                            cl->mode = Client::NORMAL;

                            Command c;
                            c.clientFd = cl->fd;
                            c.cmd = "UPLOAD_DONE";
                            c.arg = cl->uploadFilename;
                            cmd->add(c);
                        }

                        continue;
                    }
                    cl->recvBuffer.append(buf, rc);

                    size_t pos;
                    while ((pos = cl->recvBuffer.find('\n')) != string::npos) {

                        string line = cl->recvBuffer.substr(0, pos);
                        cl->recvBuffer.erase(0, pos + 1);

                        if (line.rfind("UPLOAD ", 0) == 0) {
                            size_t sp1 = line.find(' ');
                            size_t sp2 = line.find(' ', sp1 + 1);

                            if(sp2 == string::npos)
                                continue;

                            cl->uploadFilename = line.substr(sp1 + 1, sp2 - sp1 - 1);
                            //cl->uploadBytesLeft = stoll(line.substr(sp2 + 1));
                            string sizeStr = line.substr(sp2 + 1);
                            cout<<"sizeStr: "<<sizeStr<<endl;
                            cl->uploadBytesLeft = stoll(sizeStr);

                            cl->uploadFd = open(cl->uploadFilename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);

                            cl->mode = Client::UPLOAD;

                            continue;
                        }

                        Command c;
                        c.clientFd = cl->fd;

                        size_t spacePos = line.find(' ');
                        if (spacePos != string::npos) {
                            c.cmd = line.substr(0, spacePos);
                            c.arg = line.substr(spacePos + 1);
                        } else {
                            c.cmd = line;
                            c.arg = "";
                        }

                        cmd->add(c);
                    }  
                }
                if(events[i].events & EPOLLOUT) {
                    if (!this->clients.count(events[i].data.fd))
                        continue;
                    auto& cl = this->clients[events[i].data.fd];
                    lock_guard<mutex> lock(cl->cm);

                    while(!cl->q.empty()) {
                        auto& frame = cl->q.front();
                        int n = write(events[i].data.fd, frame.data(), frame.size());
                        if (n < 0) {
                            if (errno == EAGAIN || errno == EWOULDBLOCK)
                                break;
                            close(events[i].data.fd);
                            delete cl;
                            epoll_ctl(this->efd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                            this->clients.erase(events[i].data.fd);
                            break;
                        }
                        frame.erase(frame.begin(), frame.begin() + n);
                        if(frame.empty())
                            cl->q.pop();
                    }

                    if (cl->q.empty()) {
                        event.events = EPOLLIN;
                        event.data.fd = events[i].data.fd;
                        epoll_ctl(this->efd, EPOLL_CTL_MOD, events[i].data.fd, &event);
                    }
                }
            }
        }
    }
}
void Multiplexer::notifyWritable(int fd) {
    epoll_event ev{};
    ev.events = EPOLLIN | EPOLLOUT;
    ev.data.fd = fd;
    epoll_ctl(efd, EPOLL_CTL_MOD, fd, &ev);
}