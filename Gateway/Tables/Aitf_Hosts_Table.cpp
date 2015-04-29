#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "Aitf_Hosts_Table.hpp"
#include "../logger.hpp"

Aitf_Hosts_Table::Aitf_Hosts_Table(){
	/*Filter_Info info;
	info.from_amount = 0;
	info.from_limit = 2;
	Filter_Info info2;
	info2.from_amount = 0;
	info2.from_limit = 2;
	hosts[99] = info;
	hosts[2] = info2;*/
}

void Aitf_Hosts_Table::add_host(uint32_t host_ip, int limit){
	Filter_Info info;
	info.from_amount = 0;
	info.from_limit = limit;
	hosts[host_ip] = info;
}

bool Aitf_Hosts_Table::check_from_rate(uint32_t ip){

	//lock the table
	table_mutex.lock();

	bool allowed = false;

	log(logDEBUG) << "Checking from rate for " << ip;

	//check if the ip is in the table
	if(hosts.count(ip) == 1){
		Filter_Info host = hosts[ip];

		//check that the host hasnt exceeded limit
		if(host.from_amount < host.from_limit){
			log(logDEBUG2) << "Allowing request for " << ip;

			//increment the counter and allow the request
			hosts[ip].from_amount++;
			allowed = true;

			//set the callback to decrement the value
			boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(table_io, boost::posix_time::seconds(1)));
			timer->async_wait(boost::bind(&Aitf_Hosts_Table::decrement_from, this, boost::asio::placeholders::error, timer, ip));
		}
		else{
			log(logDEBUG2) << "Denying request for " << ip;
		}
	}
	else{
		log(logDEBUG2) << "Denying request for " << ip;
	}

	//unlock the table
	table_mutex.unlock();

	return allowed;
}

bool Aitf_Hosts_Table::contains_host(uint32_t ip){
	table_mutex.lock();
	bool result = (hosts.count(ip) != 0);
	table_mutex.unlock();
	return result;
}

//Called by the async timer on the seperate thread.
//Decrements the from count for the specified thread.
void Aitf_Hosts_Table::decrement_from(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, uint32_t ip){
	//free the timer
	timer.reset();

	//lock the table
	table_mutex.lock();
	log(logDEBUG2) << "Decrementing from " << ip;
	hosts[ip].from_amount--;

	//unlock the table
	table_mutex.unlock();
}
