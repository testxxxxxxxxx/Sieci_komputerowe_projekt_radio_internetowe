#ifndef AUDIO_HPP
#define AUDIO_HPP
#include <iostream>
#include "Playlist.hpp"
#include <vector>
#include "Client.hpp"
#include "Multiplexer.hpp"

using namespace std;
using namespace Song;

namespace Threads {

	class Audio {
		public:

			void run(Song::Playlist* pl, unordered_map<int, Clients::Client*>& clients);
	};
};
#endif
