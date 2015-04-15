#include "Filter_Table.hpp"
#include "../logger.hpp"



void Filter_Table::add_temp_filter(uint8_t flow[]){

	table_mutex.lock();
	//pullout dest ip
	uint32_t dst_ip;
	memcpy(&dst_ip, &flow[77], 4);
	log(logDEBUG) << "Adding temp filter for " << dst_ip;

	//if there are no existing filters for the destination
	//create a filter set for it
	if(filters.count(dst_ip) == 0){
		filters[dst_ip] = new Filter_Set(&table_mutex, &table_io);
	}
	filters[dst_ip]->add_temp_filter(flow);

	table_mutex.unlock();
}

