#include <iostream>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>

using namespace std;

namespace Song {

	class Playlist {
		queue<string> q;
		mutex c;
		condition_variable cv;
		public:

			Playlist() {}
			virtual ~Playlist() = default;

			void add(string songName);
			bool next();
			string currentBlocking();
			bool empty();
			string remove();
	};
};
