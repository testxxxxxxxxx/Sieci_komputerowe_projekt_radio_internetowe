#include <iostream>
#include "../include/CommandProvider.hpp"

using namespace std;
using namespace CommandsLogic;
using namespace Clients;

void CommandProvider::run(Playlist* p, CommandQueue* cmd, unordered_map<int, Client*>& clients, Multiplexing::Multiplexer* mux) {
	while(1) {
		Command c = cmd->remove();
		
		if(c.cmd == "ADD" || c.cmd == "UPLOAD_DONE") {
			p->add(c.arg);
			cout<<"UPLOAD_DONE"<<endl;
		}
		else if(c.cmd == "NEXT") {
			for (auto& [fd, cl] : clients) {
                lock_guard<mutex> lock(cl->cm);
				while(!cl->q.empty()) 
					cl->q.pop();
                cl->qText.push("FLUSH\n");
                cl->write = true;
                mux->notifyWritable(fd);
            }
			p->next();
			cout<<"NEXT"<<endl;		
		}
		else if(c.cmd == "REMOVE") {
			p->removeLast();
			cout<<"REMOVE"<<endl;
		}
		else if(c.cmd == "LIST") {
			auto list = p->list();
			
			for(auto& [fd, cl]: clients) {
				lock_guard<mutex> lock(cl->cm);
				for(auto& s : list)
					cl->qText.push(s + "PLAYLIST\n");
				mux->notifyWritable(fd);
			}
			cout<<"LIST"<<endl;
			/*Client* cl = clients[c.clientFd];
			lock_guard<mutex> lock(cl->cm);
			for(auto& s : list)
				cl->qText.push(s + "PLAYLIST\n");
			mux->notifyWritable(c.clientFd);*/
		}
	}
}
