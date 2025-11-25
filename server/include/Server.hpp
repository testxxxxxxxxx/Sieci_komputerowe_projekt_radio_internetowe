#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace SocketServer {

    class Server {

        int port;
        sockaddr_in saddr, caddr;
        
        public:

            int fd;

            Server(int port): port(port) {
                this->saddr = {};
                this->caddr = {};            
            }
            virtual ~Server() = default;

            void init();
            int acceptConnection();

    };
};
