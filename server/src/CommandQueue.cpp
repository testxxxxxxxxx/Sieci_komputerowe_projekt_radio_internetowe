#include <iostream>
#include "./include/CommandQueue.hpp"

using namespace std;
using namespace Commands;

void CommandQueue::add(const Command& cmd) {
	lock_guard<mutex> lock(this->mutexC);
	this->q.push(cmd);
	this->cv.notify_one();
}
Command CommandQueue::remove() {
	unique_lock<mutex> lock(this->mutexC);
	this->cv.wait(lock, [&]{return !this->q.empty();});
	Command cmd = this->q.front();
	this->q.pop();
	return c;
}
