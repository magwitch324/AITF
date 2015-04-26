#include "Filter_Table.hpp"
#include "../logger.hpp"


/*Filter_Table::~Filter_Table(){
	filters.erase(filters.begin(), filters.end());
}*/

void Filter_Table::add_temp_filter(std::vector<uint8_t> flow){

	table_mutex.lock();
	//pullout dest ip
	uint32_t dst_ip;
	memcpy(&dst_ip, &flow[77], 4);
	log(logDEBUG) << "Adding temp filter for " << dst_ip;

	//if there are no existing filters for the destination
	//create a filter set for it
	if(filters.count(dst_ip) == 0){
		Filter_Set tmp(&table_mutex, &table_io);
		filters[dst_ip] = tmp;
	}
	filters[dst_ip].add_temp_filter(flow);

	table_mutex.unlock();
}

void Filter_Table::add_long_filter(std::vector<uint8_t> flow){

	//pullout dest ip
	uint32_t dst_ip;
	memcpy(&dst_ip, &flow[77], 4);

	table_mutex.lock();
	log(logDEBUG) << "Adding long filter for " << dst_ip;

	//if there are no existing filters for the destination
	//create a filter set for it
	if(filters.count(dst_ip) == 0){
		Filter_Set tmp(&table_mutex, &table_io);
		filters[dst_ip] = tmp;
	}
	filters[dst_ip].add_long_filter(flow);

	table_mutex.unlock();
}

bool Filter_Table::is_flow_filtered(std::vector<uint8_t> flow){
	bool is_filtered = false;
	//pullout dest ip
	uint32_t dst_ip;
	memcpy(&dst_ip, &flow[77], 4);

	table_mutex.lock();
	if(filters.count(dst_ip) == 1){
		is_filtered = filters[dst_ip].is_flow_filtered(flow);
	}
	table_mutex.unlock();

	return is_filtered;
}

