#include <iostream>
#include "../include/Playlist.hpp"

using namespace std;
using namespace Song;

void Playlist::add(string name) {
	lock_guard<mutex> lock(this->c);
	this->q.push_back(name);
	this->cv.notify_one();
}
bool Playlist::next() {
	lock_guard<mutex> lock(this->c);
	if(this->q.empty())
		return false;
	this->q.pop_front();
	this->changed.store(true);
	this->ch = true;
	//cout<<this->changed<<endl;
	this->cv.notify_one();
	return true;
}
string Playlist::currentBlocking() {
	unique_lock<mutex> lock(this->c);
	this->cv.wait(lock, [&]{return !this->q.empty();});
	return this->q.front();
}
bool Playlist::empty() {
	lock_guard<mutex> lock(this->c);
	return this->q.empty();
}
bool Playlist::removeLast() {
	lock_guard<mutex> lock(this->c);
	if(this->q.empty() || this->q.size() == 1)
		return false;
	this->q.pop_back();
	this->cv.notify_one();
	return true;
}
vector<string> Playlist::list() {
	lock_guard<mutex> lock(this->c);
	return vector<string>(this->q.begin(), this->q.end());
}