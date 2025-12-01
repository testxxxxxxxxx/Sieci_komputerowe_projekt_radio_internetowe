#include "Server.hpp"
#include "CommandQueue.hpp"
#include <sys/epoll.h>

using namespace SocketServer;
using namespace Commands; 

namespace Multiplexing {

    class Multiplexer {

        int efd, maxEvents;
        Server* s;

        public:

                Multiplexer(Server* s, int maxEvents) : s(s), maxEvents(maxEvents) {
                }
                ~Multiplexer() {
                    delete s;
                }

                void init();
                void loopEvent(CommandQueue* cmd);
    };
};
