#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "Aitf_Hosts_Table.hpp"
#include "logger.hpp"

Aitf_Hosts_Table::Aitf_Hosts_Table(){
	Filter_Info* info = new Filter_Info();
	info->from_amount = 0;
	info->from_limit = 2;
	hosts[99] = info;
}

Aitf_Hosts_Table::~Aitf_Hosts_Table(){
	delete(hosts[99]);
}

bool Aitf_Hosts_Table::check_from_rate(uint32_t ip){
	
	//lock the table
	table_mutex.lock();

	bool allowed = false;

	log(logDEBUG) << "Checking from rate for " << ip;

	//check if the ip is in the table
	if(hosts.count(ip) == 1){
		Filter_Info* host = hosts[ip];

		//check that the host hasnt exceeded limit
		if(host->from_amount < host->from_limit){
			log(logDEBUG2) << "Allowing request for " << ip;

			//increment the counter and allow the request
			host->from_amount++;
			allowed = true;

			//set the callback to decrement the value
			boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(table_io, boost::posix_time::seconds(1)));
			timer->async_wait(boost::bind(&Aitf_Hosts_Table::decrement_from, this, boost::asio::placeholders::error, timer, ip));
		}
		else{
			log(logDEBUG2) << "Denying request for " << ip;
		}
	}

	//unlock the table
	table_mutex.unlock();

	return allowed;
}

//Called by the async timer on the seperate thread.
//Decrements the from count for the specified thread.
void Aitf_Hosts_Table::decrement_from(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, uint32_t ip){
	//free the timer
	timer.reset();

	//lock the table
	table_mutex.lock();
	log(logDEBUG2) << "Decrementing from " << ip;
	hosts[ip]->from_amount--;

	//unlock the table
	table_mutex.unlock();
}

//Starts the decrement thread for the table
void Aitf_Hosts_Table::start_thread(){
	table_thread = boost::thread(&Aitf_Hosts_Table::run, this);
}

//Stops the iowork and joins the thread
void Aitf_Hosts_Table::stop_thread(){
	log(logINFO) << "stopping table thread";
	table_work.reset();
	log(logDEBUG) << "work reset";
	table_thread.join();
	log(logINFO) << "table thread stopped";
}

//Starts the iowork
void Aitf_Hosts_Table::run(){
	table_work.reset(new boost::asio::io_service::work(table_io));
	table_io.run();
}