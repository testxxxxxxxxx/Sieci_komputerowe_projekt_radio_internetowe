#ifndef COMMAND_PROVIDER_HPP
#define COMMAND_PROVIDER_HPP
#include "Playlist.hpp"
#include "CommandQueue.hpp"

using namespace Song;
using namespace Commands;

namespace CommandsLogic {
	
	class CommandProvider {
		public:

			void run(Song::Playlist* p, Commands::CommandQueue* cmd);
	};
};
#endif