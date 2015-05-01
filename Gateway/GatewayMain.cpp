#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "logger.hpp"
#include "Hasher.hpp"
#include "Aitf_Manager/Aitf_Manager.hpp"
#include "Internet_Manager/Internet_Manager.hpp"
#include "Tables/Aitf_Hosts_Table.hpp"
#include "Tables/Nonaitf_Dests_Table.hpp"


std::string* gateway_key;
loglevel_e loglevel = logERROR;
Aitf_Hosts_Table* aitf_hosts_table;
Filter_Table* filter_table;
Filter_Table* shadow_table;
Nonaitf_Dests_Table* nonaitf_dests_table;
uint32_t MY_IP = 101;

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

void set_gateway_values(int x){
	gateway_key = new std::string(std::to_string(x));

	//add any non AITF enabled hosts
	nonaitf_dests_table->add_dst(16777343);
	uint32_t ip = inet_addr("10.10.128.129");
	nonaitf_dests_table->add_dst(ip);
	//aitf_hosts_table->add_host(99, 10);
	switch(x){
		case 1:
			MY_IP = inet_addr("10.4.13.3");
			aitf_hosts_table->add_host(inet_addr("10.4.13.1"), 10);
			aitf_hosts_table->add_host(inet_addr("10.4.13.5"), 10);
			aitf_hosts_table->add_host(inet_addr("10.4.13.3"), 10);
			aitf_hosts_table->add_host(inet_addr("10.4.13.4"), 10);
		break;
		case 2:
			MY_IP = inet_addr("10.4.13.4");
			aitf_hosts_table->add_host(inet_addr("10.4.13.3"), 10);
			aitf_hosts_table->add_host(inet_addr("10.4.13.5"), 10);
		break;
		case 3:
			MY_IP = inet_addr("10.4.13.5");
			aitf_hosts_table->add_host(inet_addr("10.4.13.7"), 10);
			aitf_hosts_table->add_host(inet_addr("10.4.13.3"), 10);
			aitf_hosts_table->add_host(inet_addr("10.4.13.4"), 10);
		break;
		/*case 4:
			MY_IP = inet_addr("10.4.13.4");
			aitf_hosts_table->add_host(inet_addr("10.4.13.1"), 10);
			aitf_hosts_table->add_host(inet_addr("10.4.13.2"), 10);
			aitf_hosts_table->add_host(inet_addr("10.4.13.3"), 10);
			aitf_hosts_table->add_host(inet_addr("10.4.13.6"), 10);
		break;*/
	}
}

/*
   Main function for the Gateway. Initaites the AITF manager and
   the internet traffic manager
 */
int main(int argc, char* argv[]){

	set_log_level(4);

	aitf_hosts_table = new Aitf_Hosts_Table();
	aitf_hosts_table->start_thread();
	filter_table = new Filter_Table();
	filter_table->start_thread();
	shadow_table = new Filter_Table();
	shadow_table->start_thread();
	nonaitf_dests_table = new Nonaitf_Dests_Table();


	//set the gateway info
	if(argc == 2){
		set_gateway_values(atoi(argv[1]));
	}
	else{
		set_gateway_values(1);
	}


	Aitf_Manager* aitf_manager = new Aitf_Manager();
	aitf_manager->start_thread();

	Internet_Manager* i_manager = new Internet_Manager();
	i_manager->start_thread();



	int x;
	std::cin >> x;

	//stop primary threads
	aitf_manager->stop_thread();
	i_manager->stop_thread();

	//stop table threads
	aitf_hosts_table->stop_thread();
	filter_table->stop_thread();
	shadow_table->stop_thread();

	//cleanup
	delete(aitf_manager);
	delete(i_manager);

	delete(aitf_hosts_table);
	delete(filter_table);
	delete(shadow_table);
	delete(nonaitf_dests_table);

	delete(gateway_key);

}




