#include "Server.hpp"
#include "CommandQueue.hpp"
#include <sys/epoll.h>
#include <unordered_map>

using namespace SocketServer;
using namespace Commands; 

namespace Multiplexing {

    struct Client {
        int fd;
        queue<vector<int>> q;
        mutex cm;
        bool write = false;
        string recvBuffer;       
        Client(int _fd): fd(fd) {}        
    };

    class Multiplexer {

        int efd, maxEvents;
        Server* s;
        unordered_map<int, Client*> clients;

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
