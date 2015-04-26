#include "Filter_Table.hpp"
#include "../logger.hpp"


/*Filter_Table::~Filter_Table(){
  filters.erase(filters.begin(), filters.end());
  }*/

void Filter_Table::add_temp_filter(Flow flow){

	table_mutex.lock();

	log(logDEBUG) << "Adding temp filter for " << flow.dst_ip;

	//if there are no existing filters for the destination
	//create a filter set for it
	if(filters.count(flow.dst_ip) == 0){
		Filter_Set tmp(&table_mutex, &table_io);
		filters[flow.dst_ip] = tmp;
	}
	filters[flow.dst_ip].add_temp_filter(flow);

	table_mutex.unlock();
}

void Filter_Table::add_long_filter(Flow flow){

	table_mutex.lock();
	log(logDEBUG) << "Adding long filter for " << flow.dst_ip;

	//if there are no existing filters for the destination
	//create a filter set for it
	if(filters.count(flow.dst_ip) == 0){
		Filter_Set tmp(&table_mutex, &table_io);
		filters[flow.dst_ip] = tmp;
	}
	filters[flow.dst_ip].add_long_filter(flow);

	table_mutex.unlock();
}

bool Filter_Table::is_flow_filtered(Flow flow){
	bool is_filtered = false;
	
	table_mutex.lock();
	if(filters.count(flow.dst_ip) == 1){
		is_filtered = filters[flow.dst_ip].is_flow_filtered(flow);
	}
	table_mutex.unlock();

	return is_filtered;
}

