/*
 * HostMain.cpp
 *
 */

#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
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
	log(logINFO) << "Starting Main";

	set_log_level(4);


	PacketManager * pm = new PacketManager();

	int x;
	std::cin >> x;

	delete pm;

	log(logINFO) << "Finishing Main";

}




