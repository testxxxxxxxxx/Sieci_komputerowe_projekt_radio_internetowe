#include "Server.hpp"
#include "CommandQueue.hpp"
#include <sys/epoll.h>
#include <unordered_map>
#include "Client.hpp"

using namespace SocketServer;
using namespace Commands; 
using namespace Clients;

namespace Multiplexing {

    /*struct Client {
        int fd;
        queue<vector<int>> q;
        mutex cm;
        bool write = false;
        string recvBuffer;
        enum Mode { NORMAL, UPLOAD } mode = NORMAL;

        string uploadFilename;
        long long uploadBytesLeft = 0;
        int uploadFd = -1;       
        Client(int _fd): fd(fd) {}        
    };*/

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
                void Multiplexer::broadcast(const vector<uint8_t>& frame);
    };
};
