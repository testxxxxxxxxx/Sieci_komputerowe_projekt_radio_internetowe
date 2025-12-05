#include <queue>
#include <vector>
#include <mutex>

namespace Clients {
    class Client {
        public:
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
    };
};
