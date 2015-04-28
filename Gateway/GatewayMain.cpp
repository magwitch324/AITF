#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "logger.hpp"
#include "Hasher.hpp"
#include "Aitf_Manager/Aitf_Manager.hpp"
#include "Internet_Manager/Internet_Manager.hpp"
#include "Tables/Aitf_Hosts_Table.hpp"


std::string* gateway_key;
loglevel_e loglevel = logERROR;
Aitf_Hosts_Table* aitf_hosts_table;
Filter_Table* filter_table;
Filter_Table* shadow_table;

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
int main(int argc, char* argv[]){

	set_log_level(4);

	//grab the key value
	/*if(argc == 2){
	  gateway_key = new std::string(argv[1]);
	  }
	  else{
	  gateway_key = new std::string("123456");
	  }

	  aitf_hosts_table = new Aitf_Hosts_Table();
	  aitf_hosts_table->start_thread();
	  filter_table = new Filter_Table();
	  filter_table->start_thread();
	  shadow_table = new Filter_Table();
	  shadow_table->start_thread();

	  Aitf_Manager* aitf_manager = new Aitf_Manager();
	  aitf_manager->start_thread();



	  int x;
	  std::cin >> x;

	//stop primary threads
	aitf_manager->stop_thread();

	//stop table threads
	aitf_hosts_table->stop_thread();
	filter_table->stop_thread();
	shadow_table->stop_thread();

	//cleanup
	delete(aitf_manager);

	delete(aitf_hosts_table);
	delete(filter_table);
	delete(shadow_table);

	delete(gateway_key);*/



	Internet_Manager* i_manager = new Internet_Manager();
	i_manager->start_thread();

	int x;
	std::cin >> x;

	i_manager->stop_thread();
	delete(i_manager);

}




