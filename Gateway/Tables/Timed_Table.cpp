#include "Timed_Table.hpp"
#include "../logger.hpp"


//Starts the decrement thread for the table
void Timed_Table::start_thread(){
	log(logINFO) << "Starting table thread";
	table_thread = boost::thread(&Timed_Table::run, this);
}

//Stops the iowork and joins the thread
void Timed_Table::stop_thread(){
	log(logINFO) << "stopping table thread";
	table_work.reset();
	log(logDEBUG) << "work reset";
	table_thread.join();
	log(logINFO) << "table thread stopped";
}

//Starts the iowork
void Timed_Table::run(){
	table_work.reset(new boost::asio::io_service::work(table_io));
	table_io.run();
}