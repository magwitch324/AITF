#include "Filter_Set.hpp"
#include "../logger.hpp"

Flow::Flow(uint8_t new_flow[]){
	std::vector<uint8_t> tmp(new_flow, new_flow+81);
	flow = tmp;
}

Filter_Set::Filter_Set(boost::mutex* mutex_, boost::asio::io_service* table_io_){
	table_mutex = mutex_;
	table_io = table_io_;
}

void Filter_Set::add_temp_filter(uint8_t flow[]){
	add_filter(flow, TEMP_TIME);
}

void Filter_Set::add_long_filter(uint8_t flow[]){
	add_filter(flow, LONG_TIME);
}

void Filter_Set::add_filter(uint8_t flow[], int secs){

	log(logDEBUG2) << "In filterset add filter " << secs;
	//determine the type of filter
	//pull out the source
	uint32_t src_ip;
	memcpy(&src_ip, &flow[0], 4);

	//if the filter is a * filter
	if(src_ip == 0){


		//pullout the last gateway IP
		uint32_t gtw_ip;
		memcpy(&gtw_ip, &flow[5], 4);
		log(logINFO) << "Creating * filter for " << gtw_ip;

		//check if there is already a filter made
		if(gateway_filters.count(gtw_ip) == 0){
			gateway_filters[gtw_ip] = 0;
		}

		//increment the filter requests value
		gateway_filters[gtw_ip]++;

		//add callback
		boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(*table_io, boost::posix_time::seconds(secs)));
		timer->async_wait(boost::bind(&Filter_Set::decrement_gateway_filter, this, boost::asio::placeholders::error, timer, secs, gtw_ip));


	}
	else{
		//add to the flow_filters
		Flow* c_flow = new Flow(flow);

		//check if there is already a filter in place
		if(flow_filters.count(*c_flow) == 0){
			flow_filters[*c_flow] = 0;
		}

		//increment requests for this filter
		flow_filters[*c_flow]++;

		log(logINFO) << "Filter made for " << src_ip << ". count at " << flow_filters[*c_flow];

		//add callback
		boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(*table_io, boost::posix_time::seconds(secs)));
		timer->async_wait(boost::bind(&Filter_Set::decrement_flow_filter, this, boost::asio::placeholders::error, timer, secs, c_flow));

	}
}

bool Filter_Set::is_flow_filtered(uint8_t flow[]){
	log(logDEBUG2) << "in filterset is flow filtered";
	bool is_filtered = false;
	//determine the type of filter
	//pull out the source
	uint32_t src_ip;
	memcpy(&src_ip, &flow[0], 4);

	//if the filter is a * filter
	if(src_ip == 0){

		//pullout the last gateway IP
		uint32_t gtw_ip;
		memcpy(&gtw_ip, &flow[5], 4);

		is_filtered = (gateway_filters.count(gtw_ip) == 1);
	}
	else{
		log(logDEBUG2) << "creating flow";
		Flow* c_flow = new Flow(flow);
		log(logDEBUG2) << "checking table";
		flow_filters.count(*c_flow);
		//is_filtered = (flow_filters.count(*c_flow) == 1);
		log(logDEBUG2) << "deleting flow";
		delete(c_flow);
	}

	return is_filtered;

}


void Filter_Set::decrement_flow_filter(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, int secs, Flow* flow){
	//reset the timer
	timer.reset();

	//lock the table
	table_mutex->lock();

	//check to make sure the flow exists
	if(flow_filters.count(*flow) == 1){

		//check if this decrement should remove the filter
		if(flow_filters[*flow] == 1){
			log(logDEBUG) << "Deleting filter";
			flow_filters.erase(*flow);
		}
		else{
			log(logDEBUG) << "Decrementing filter";
			flow_filters[*flow]--;
		}
	}
	else{
		log(logERROR) << "Flow does not exits!!!!!";
	}
	delete(flow);
	table_mutex->unlock();
}

void Filter_Set::decrement_gateway_filter(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, int secs, uint32_t gtw_ip){
	//reset the timer
	timer.reset();

	//lock the table
	table_mutex->lock();

	//check to make sure the gateway ip exists
	if(gateway_filters.count(gtw_ip) == 1){

		//check if this decrement should remove the filter
		if(gateway_filters[gtw_ip] == 1){
			log(logDEBUG) << "Deleting filter";
			gateway_filters.erase(gtw_ip);
		}
		else{
			log(logDEBUG) << "Decrementing filter";
			gateway_filters[gtw_ip]--;
		}
	}
	else{
		log(logERROR) << "Gateway ip does not exits!!!!!";
	}

	table_mutex->unlock();
}
