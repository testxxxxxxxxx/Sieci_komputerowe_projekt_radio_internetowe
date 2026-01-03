#include <iostream>
#include "../include/CommandProvider.hpp"

using namespace std;
using namespace CommandsLogic;
using namespace Clients;

void CommandProvider::run(Playlist* p, CommandQueue* cmd, unordered_map<int, Client*>& clients, Multiplexing::Multiplexer* mux) {
	while(1) {
		Command c = cmd->remove();
		
		if(c.cmd == "ADD" || c.cmd == "UPLOAD_DONE")
			p->add(c.arg);
		else if(c.cmd == "NEXT")
			p->next();	
		else if(c.cmd == "REMOVE")
			p->removeLast();
		else if(c.cmd == "LIST") {
			auto list = p->list();
			
			Client* cl = clients[c.clientFd];
			lock_guard<mutex> lock(cl->cm);
			for(auto& s : list)
				cl->qText.push(s + "PLAYLIST\n");
			mux->notifyWritable(c.clientFd);
		}
	}
}
