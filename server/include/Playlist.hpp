#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP
#include <iostream>
#include <deque>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>

using namespace std;

namespace Song {

	class Playlist {
		deque<string> q;
		mutex c;
		condition_variable cv;
		public:
			atomic<bool> changed{false};
			bool ch = false;

			Playlist() {}
			virtual ~Playlist() = default;

			void add(string songName);
			bool next();
			string currentBlocking();
			bool empty();
			bool removeLast();
			vector<string> list();	
	};
};
#endif