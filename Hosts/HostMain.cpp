/*
 * HostMain.cpp
 *
 */

#include <string>

#include "logger.hpp"

#include "Managers/PacketManager.hpp"
#include "Modules/FilterModule.hpp"
#include "Modules/PolicyModule.hpp"
#include "Servers/Udp_Server.hpp"

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

int main(int argc, char **argv){
	int i;
	char command[200];
	PacketManager * pms[argc-1];

	PolicyModule * policy = new PolicyModule();
	FilterModule * filter = new FilterModule();

	llog(logINFO) << "Starting Main";
	set_log_level(4);

	Udp_Server * udps;
	udps = udps->getInstance();

	for ( i = 1; i < argc; i ++ ) {
		printf ("sudo iptables -D INPUT -d %s -j NFQUEUE --queue-num %u \n", argv[i], i*2+1);
		printf ("sudo iptables -D OUTPUT -s %s -j NFQUEUE --queue-num %u \n", argv[i], i*2+2);

		sprintf( command, "iptables -A INPUT -d %s -j NFQUEUE --queue-num %u", argv[i], i*2+1 );
		system( command );
		sprintf( command, "iptables -A OUTPUT -s %s -j NFQUEUE --queue-num %u", argv[i], i*2+2 );
		system( command );
		pms[i-1] = new PacketManager( inet_addr(argv[i]), i*2+1, i*2+2, policy, filter);
	}

	udps->start();

	int x;
	std::cin >> x;

	udps->stop();

	for ( i = 1; i < argc; i ++ ) {
		delete pms[i-1];
		sprintf( command, "iptables -D OUTPUT -s %s -j NFQUEUE --queue-num %u", argv[i], i*2+2 );
		system( command );
		sprintf( command, "iptables -D INPUT -d %s -j NFQUEUE --queue-num %u", argv[i], i*2+1 );
		system( command );
	}

	delete policy;
	delete filter;

	llog(logINFO) << "Finishing Main";

}



