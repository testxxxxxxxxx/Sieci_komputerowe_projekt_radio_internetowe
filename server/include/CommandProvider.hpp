#include "./include/Playlist.hpp"
#include "./include/CommandQueue.hpp"

using namespace Song;
using namespace Commands;

namespace CommandsLogic {
	
	class CommandProvider {
		public:

			static void run(Playlist* p, CommandQueee* cmd);
	};
};
