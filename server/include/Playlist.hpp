#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP
#include <iostream>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>

using namespace std;

namespace Song {

	class Playlist {
		queue<string> q;
		mutex c;
		condition_variable cv;
		public:
			atomic<bool> changed{false};

			Playlist() {}
			virtual ~Playlist() = default;

			void add(string songName);
			bool next();
			string currentBlocking();
			bool empty();
			string remove();
	};
};
#endif