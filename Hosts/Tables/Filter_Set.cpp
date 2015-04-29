#include "Filter_Set.hpp"
#include "../logger.hpp"

Filter_Set::Filter_Set(boost::mutex* mutex_, boost::asio::io_service* table_io_){
	table_mutex = mutex_;
	table_io = table_io_;
}

Filter_Set::Filter_Set(){

}

void Filter_Set::add_temp_filter(Flow flow){
	add_filter(flow, TEMP_TIME);
}

void Filter_Set::add_long_filter(Flow flow){
	add_filter(flow, LONG_TIME);
}

void Filter_Set::add_filter(Flow flow, int secs){

	log(logDEBUG2) << "In filterset add filter " << secs;
	//determine the type of filter

	//if the filter is a * filter
	if(flow.src_ip == 0){

		log(logINFO) << "Creating * filter for " << flow.gtw0_ip;

		//check if there is already a filter made
		if(gateway_filters.count(flow.gtw0_ip) == 0){
			gateway_filters[flow.gtw0_ip] = 0;
		}

		//increment the filter requests value
		gateway_filters[flow.gtw0_ip]++;

		//add callback
		boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(*table_io, boost::posix_time::seconds(secs)));
		timer->async_wait(boost::bind(&Filter_Set::decrement_gateway_filter, this, boost::asio::placeholders::error, timer, secs, flow.gtw0_ip));


	}
	else{
		//add to the flow_filters
		//Flow* c_flow = new Flow(flow);
		boost::shared_ptr<Flow> c_flow(new Flow(flow));

		//check if there is already a filter in place
		if(flow_filters.count(*c_flow) == 0){
			flow_filters[*c_flow] = 0;
		}

		//increment requests for this filter
		flow_filters[*c_flow]++;

		log(logINFO) << "Filter made for " << flow.src_ip << ". count at " << flow_filters[*c_flow];

		//add callback
		boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(*table_io, boost::posix_time::seconds(secs)));
		timer->async_wait(boost::bind(&Filter_Set::decrement_flow_filter, this, boost::asio::placeholders::error, timer, secs, c_flow));

	}
}

void Filter_Set::add_gtw_rvalue(uint32_t gtw_ip, uint64_t rvalue){
	rvalue_filters[gtw_ip] = rvalue;
}

int Filter_Set::attempt_count(Flow flow){
	log(logDEBUG2) << "in filterset is flow filtered";
	int attempt_count = 0;
	//determine the type of filter

	//if the filter is a * filter and the filter exits
	if(flow.src_ip == 0 && gateway_filters.count(flow.gtw0_ip) == 1){
		attempt_count = gateway_filters[flow.gtw0_ip];
	}
	else{
		log(logDEBUG2) << "checking table";
		if(flow_filters.count(flow) == 1){
			attempt_count = flow_filters[flow];
		}
	}

	return attempt_count;

}

bool Filter_Set::flow_is_filtered(Flow flow){
	bool is_filtered = false;
	//determine the type of flow
	//if it is a * flow
	if(flow.src_ip == 0){
		is_filtered = (gateway_filters.count(flow.gtw0_ip) == 1);
	}
	else{
		is_filtered = (flow_filters.count(flow) == 1);
	}

	//if there is no regular filter for it check the rvals
	if(!is_filtered){
		//now check for valid rvalues
		for(int i = 0; i < 6; i++){
			//pull out the ip and rvalue
			uint32_t ip = flow.get_gtw_ip_at(i);
			uint64_t rvalue = flow.get_gtw_rvalue_at(i);

			//if there is a recorded rvalue for this gateway
			if(rvalue_filters.count(ip) == 1){
				//check the rvalue
				if(rvalue != rvalue_filters[ip]){
					is_filtered = true;
					break;
				}
			}
		}//for
	}//if(!isfiltered)

	return is_filtered;


}


void Filter_Set::decrement_flow_filter(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, int secs, boost::shared_ptr<Flow> flow){
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
