#include <iostream>
#include "../include/Server.hpp"
#include "../include/Multiplexer.hpp"
#include "../include/CommandProvider.hpp"
#include "../include/Playlist.hpp"
#include "../include/Audio.hpp"
#include <thread>

#define MAX_CONN 10

using namespace std;
using namespace Multiplexing;
using namespace CommandsLogic;
using namespace SocketServer;

int main(int argc, char *argv[]) {
	if(argc < 2) {
		perror("Usage: ./TCPAudioServer <port>");
		return 1;
	}
	int PORT = atoi(argv[1]);
	if(PORT == 0) {
		perror("Value error: <port> is not a integer");
		return 1;
	}
	Song::Playlist pl;
	SocketServer::Server s(PORT);
	CommandsLogic::CommandProvider cp;
	Commands::CommandQueue cmd;
	Multiplexing::Multiplexer m(&s, MAX_CONN);
	s.init();
	m.init();

	thread audioThr([&]{
		Threads::Audio audio; 
		audio.run(&pl, m.clients, &m); 
	});
	thread commandThr([&]{cp.run(&pl, &cmd, m.clients, &m);});
	thread epollThr([&]{m.loopEvent(&cmd);});

	audioThr.join();
	commandThr.join();
	epollThr.join();

	return 0;
}
