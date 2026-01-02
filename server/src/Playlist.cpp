#include <iostream>
#include "../include/Playlist.hpp"

using namespace std;
using namespace Song;

void Playlist::add(string name) {
	lock_guard<mutex> lock(this->c);
	this->q.push(name);
	this->cv.notify_one();
}
bool Playlist::next() {
	lock_guard<mutex> lock(this->c);
	if(this->q.empty())
		return false;
	this->q.pop();
	cout<<"next"<<endl;
	this->changed.store(true, memory_order_release);
	cout<<this->changed<<endl;
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
string Playlist::remove() {
	lock_guard<mutex> lock(this->c);
	if(this->q.empty())
		return "";
	string song = this->q.front();
	this->q.pop();
	this->cv.notify_one();
	return song;
}