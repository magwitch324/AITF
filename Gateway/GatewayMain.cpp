#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "logger.hpp"
#include "Aitf_Manager/Aitf_Manager.hpp"

#include "Tables/Aitf_Hosts_Table.hpp"

loglevel_e loglevel = logERROR;
Aitf_Hosts_Table* aitf_hosts_table;

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

/*
   Main function for the Gateway. Initaites the AITF manager and
   the internet traffic manager
 */
int main(){

	set_log_level(4);

	aitf_hosts_table = new Aitf_Hosts_Table();
	aitf_hosts_table->start_thread();

	Aitf_Manager* aitf_manager = new Aitf_Manager();
	aitf_manager->start_thread();



	int x;
	std::cin >> x;

	//stop primary threads
	aitf_manager->stop_thread();

	//stop table threads
	aitf_hosts_table->stop_thread();

	//cleanup
	delete(aitf_manager);

	delete(aitf_hosts_table);

}




