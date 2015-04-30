#include "Filter_Table.hpp"
#include "../logger.hpp"
#include "../Helpers.hpp"


/*Filter_Table::~Filter_Table(){
  filters.erase(filters.begin(), filters.end());
  }*/

void Filter_Table::add_temp_filter(Flow flow){

	table_mutex.lock();

	log(logDEBUG) << "Adding temp filter for " << Helpers::ip_to_string(flow.dst_ip);

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
	log(logDEBUG) << "Adding long filter for " << Helpers::ip_to_string(flow.dst_ip);

	//if there are no existing filters for the destination
	//create a filter set for it
	if(filters.count(flow.dst_ip) == 0){
		Filter_Set tmp(&table_mutex, &table_io);
		filters[flow.dst_ip] = tmp;
	}
	filters[flow.dst_ip].add_long_filter(flow);

	table_mutex.unlock();
}

int Filter_Table::attempt_count(Flow flow){
	int attempt_count = 0;

	table_mutex.lock();
	if(filters.count(flow.dst_ip) == 1){
		attempt_count = filters[flow.dst_ip].attempt_count(flow);
	}
	table_mutex.unlock();

	return attempt_count;
}

void Filter_Table::add_gtw_rvalue(uint32_t dst_ip, uint32_t gtw_ip, uint64_t rvalue){

	table_mutex.lock();

	//if there are no existing filters for the destination
	//create a filter set for it
	if(filters.count(dst_ip) == 0){
		Filter_Set tmp(&table_mutex, &table_io);
		filters[dst_ip] = tmp;
	}
	filters[dst_ip].add_gtw_rvalue(gtw_ip, rvalue);
	table_mutex.unlock();
}

bool Filter_Table::flow_is_filtered(Flow flow){
	bool is_filtered = false;

	table_mutex.lock();
	if(filters.count(flow.dst_ip > 0)){
		is_filtered = filters[flow.dst_ip].flow_is_filtered(flow);
	}

	table_mutex.unlock();

	return is_filtered;
}

