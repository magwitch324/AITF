/*
 * HostMain.cpp
 *
 */

#include "logger.hpp"

#include "Managers/PacketManager.hpp"

loglevel_e loglevel = logERROR;

void set_log_level(int level){
	switch(level){
		case 0: loglevel = logERROR;
			break;
		case 1: loglevel = logWARNING;
			break;
		case 2: loglevel = logINFO;
			break;
		case 3: loglevel = logDEBUG;
			break;
		case 4: loglevel = logDEBUG2;
			break;
	}
}

int main(){
	llog(logINFO) << "Starting Main";

	set_log_level(4);


	PacketManager * pm = new PacketManager();

	int x;
	std::cin >> x;

	delete pm;

	llog(logINFO) << "Finishing Main";

}




