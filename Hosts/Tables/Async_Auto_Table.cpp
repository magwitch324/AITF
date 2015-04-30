#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "Async_Auto_Table.hpp"
#include "../logger.hpp"

Async_Auto_Table::Async_Auto_Table() {
	llog(logDEBUG) << "I am creating normal int table " ;
	print_timer = NULL;
}

Async_Auto_Table::Async_Auto_Table(std::string filename, uint32_t timeout) {

	llog(logDEBUG) << "I am creating printable int table " << filename;

	std::ofstream fh;
	fh.open(filename, std::ios::out);
	fh << "\n";
	fh.close();

	print_timer = new boost::asio::deadline_timer(table_io, boost::posix_time::milliseconds(timeout));
	print_timer->async_wait(boost::bind(&Async_Auto_Table::printStatus, this, boost::asio::placeholders::error, filename, timeout));
}

Async_Auto_Table::~Async_Auto_Table() {
	
	llog(logDEBUG) << "I am deleteing int table" ;

	if (print_timer) {
		print_timer->cancel();
		delete(print_timer);
	}

}

int Async_Auto_Table::getValue(uint32_t ip) {
	llog(logDEBUG) << "get value from flow table " << ip;
	int value = -1;
	table_mutex.lock();
	if (table.count(ip) > 0) {
		value = table[ip];
	}
	table_mutex.unlock();
	return value;
}

/*
 * addValue: adds the value
 *	input: 	ip - some arbitrary ip address
 *			value - the value to add and decrement
 *			max - some value to compare the new table value to, give a non positive if no comparison should take place
 *			timeout - the time in ms for the decrement to occur
 *	output: the given value or -1 if the max was surpassed with the new value added and not already surpassed
 */
int Async_Auto_Table::addValue(uint32_t ip, int value, int max, uint32_t timeout) {
	llog(logDEBUG) << "adding " << value << " to " << ip;
	int ret = value;
	table_mutex.lock();

	table[ip] += value;

	if ((max > 0) && (table[ip] > max) && ((table[ip] - value) <= max) )
		ret = -1;
	//llog(logINFO) << "++" << ip << "Added " << value << " to make " << table[ip];
	table_mutex.unlock();

	boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(table_io, boost::posix_time::seconds(1)));
	timer->async_wait(boost::bind(&Async_Auto_Table::decrement, this, boost::asio::placeholders::error, timer, ip, value));
	return ret;
}


void Async_Auto_Table::decrement(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, uint32_t ip, int value) {
	llog(logDEBUG) << "removing " << value << " to " << ip;
	timer.reset();

	table_mutex.lock();

	table[ip] -= value;

	//llog(logINFO) << "--" << ip << "removed " << value << " to make " << table[ip];
	if (table[ip] == 0)
		table.erase(ip);

	table_mutex.unlock();
}

void Async_Auto_Table::printStatus(const boost::system::error_code& e, std::string filename, uint32_t timeout) {

	if (e == boost::asio::error::operation_aborted) {
		llog(logDEBUG) << "Should stop printing ";
		return;
	}

	std::ofstream fh;
	fh.open(filename, std::ios::out | std::ios::app);

	table_mutex.lock();

	for ( auto iter = table.begin(); iter != table.end(); ++ iter ) {
		fh << "," << iter->first << "," << iter->second;
		//llog(logINFO) << "," << iter->first << "," << iter->second;
	}
	table_mutex.unlock();

	fh << "\n";
	fh.close();

	print_timer->expires_from_now(boost::posix_time::milliseconds(timeout));
	print_timer->async_wait(boost::bind(&Async_Auto_Table::printStatus, this, boost::asio::placeholders::error, filename, timeout));

}
