#include "./include/Server.hpp"
#include <sys/epoll.h>

using namespace SocketServer; 

namespace Multiplexing {

    class Multiplexer {

        int edf, maxEvents;
        Server* s;

        public:

                Multiplexer(Server* s, int maxEvents) : s(s), maxEvents(maxEvents) {
                }
                ~Multiplexer() {
                    delete s;
                }

                void init();
                void loopEvent();
    };
};
