#include <queue>
#include <vector>
#include <mutex>

namespace Clients {
    class Client {
        public:
            int fd;
            queue<vector<uint8_t>> q;
            bool write = false;
            mutex cm;
    };
};
