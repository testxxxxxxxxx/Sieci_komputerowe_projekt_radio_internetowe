#include <iostream>
#include <Playlist.hpp>
#include <vector>
#include "./include/Client.hpp"

using namespace std;
using namespace Song;

namespace Threads {

	class Audio {
		public:

			void run(Playlist* pl, vector<Client>& clients);
			void sendClients(mp3dec_ex_t& dec, int16_t* buffer, int size, vector<Client>& clients);
	};
};
