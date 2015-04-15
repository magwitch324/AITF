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

	log(logDEBUG2) << "In filterset add temp filter";
	//determine the type of filter
	//pull out the source
	uint32_t src_ip;
	memcpy(&src_ip, &flow[0], 4);

	//if the filter is a * filter
	if(src_ip == 0){
		log(logINFO) << "Creating filter for *";
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
		boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(*table_io, boost::posix_time::seconds(1)));
		timer->async_wait(boost::bind(&Filter_Set::decrement_flow_filter, this, boost::asio::placeholders::error, timer, c_flow));

	}
}

void Filter_Set::decrement_flow_filter(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, Flow* flow){
	//free the timer
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

	table_mutex->unlock();
}
