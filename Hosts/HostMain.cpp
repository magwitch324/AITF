/*
 * HostMain.cpp
 *
 */

#include <string>

#include <boost/thread.hpp>

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

void start_server(Udp_Server * udps) {
	udps->start();
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
		sprintf( command, "iptables -A INPUT -d %s -s 10.4.13.0/24 -j NFQUEUE --queue-num %u", argv[i], i*2+1 );
		system( command );
		sprintf( command, "iptables -A OUTPUT -s %s -d 10.4.13.0/24 -j NFQUEUE --queue-num %u", argv[i], i*2+2 );
		system( command );
		pms[i-1] = new PacketManager( inet_addr(argv[i]), i*2+1, i*2+2, policy, filter);
	}
	boost::thread udp_thread(&start_server, udps);

	int x;
	std::cin >> x;

	udps->stop();
	udp_thread.join();

	for ( i = 1; i < argc; i ++ ) {
		delete pms[i-1];
		sprintf( command, "iptables -D OUTPUT -s %s -d 10.4.13.0/24 -j NFQUEUE --queue-num %u", argv[i], i*2+2 );
		system( command );
		sprintf( command, "iptables -D INPUT -d %s -s 10.4.13.0/24 -j NFQUEUE --queue-num %u", argv[i], i*2+1 );
		system( command );
	}

	delete policy;
	delete filter;

	llog(logINFO) << "Finishing Main";

}





