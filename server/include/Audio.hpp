#include <iostream>
#include <Playlist.hpp>
#include <vector>
#include "minimp3_ex.h"
#include "Client.hpp"
#include "Multiplexer.hpp"

using namespace std;
using namespace Song;

namespace Threads {

	class Audio {
		public:

			void run(Song::Playlist* pl, unordered_map<int, Clients::Client*>& clients);
			void sendClients(mp3dec_ex_t& dec, int16_t* buffer, int size, unordered_map<int, Clients::Client*>& clients);
	};
};
