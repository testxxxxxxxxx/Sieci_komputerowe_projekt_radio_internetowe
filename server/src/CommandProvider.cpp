#include <iostream>
#include "./include/CommandProvider.hpp"

using namespace std;
using namespace CommandsLogic;

void CommandProvider::run(Playlist* p, CommandQueue* cmd) {
	while(1) {
		Command c = cmd->remove();
		
		if(c.cmd == "ADD" || c.cmd == "UPLOAD_DONE")
			p->add(c.arg);
		else if(c.cmd == "NEXT")
			p->next();
		else if(c.cmd == "REMOVE")
			p->remove();
	}
}
