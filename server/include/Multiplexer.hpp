#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP
#include "Server.hpp"
#include "CommandQueue.hpp"
#include <sys/epoll.h>
#include <unordered_map>
#include "Client.hpp"

using namespace SocketServer;
using namespace Commands; 
using namespace Clients;

namespace Multiplexing { 

    class Multiplexer {

        int efd, maxEvents;
        Server* s;

        public:
                unordered_map<int, Clients::Client*> clients; 

                Multiplexer(Server* s, int maxEvents) : s(s), maxEvents(maxEvents) {
                }
                ~Multiplexer() {
                    delete s;
                }

                void init();
                void loopEvent(CommandQueue* cmd);
                void notifyWritable(int fd);
    };
};
#endif
