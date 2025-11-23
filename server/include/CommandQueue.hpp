#include <iostream>
#include <mutex>
#include <queue>
#include <vector>
#include <condition_variable>

using namespace std;

namespace Commands {

	struct Command {
		int clientFd;
		string cmd, arg;
	};
	
	class CommandQueue {
		queue<Command> q;
		mutex mutexC;
		condition_variable cv;

		public:

			CommandQueue() {

			}
			virtual ~CommandQueue() = default;

			void add(const Command& cmd);
			Command remove();
	};
};
