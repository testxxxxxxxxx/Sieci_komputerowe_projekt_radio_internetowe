#include <iostream>
#include "../include/CommandProvider.hpp"

using namespace std;
using namespace CommandsLogic;
using namespace Clients;

void CommandProvider::run(Playlist* p, CommandQueue* cmd, unordered_map<int, Client*>& clients) {
	while(1) {
		Command c = cmd->remove();
		
		if(c.cmd == "ADD" || c.cmd == "UPLOAD_DONE")
			p->add(c.arg);
		else if(c.cmd == "NEXT"){
			p->next();
			/*for (auto& [fd, cl] : clients) {
    			lock_guard<mutex> lock(cl->cm);
    			queue<vector<uint8_t>> empty;
    			swap(cl->q, empty);
			}*/
		}
		else if(c.cmd == "REMOVE")
			p->remove();
	}
}
