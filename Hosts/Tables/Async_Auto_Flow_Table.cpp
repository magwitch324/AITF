#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "Async_Auto_Flow_Table.hpp"
#include "../logger.hpp"
#include "../Helpers.hpp"

Async_Auto_Flow_Table::Async_Auto_Flow_Table() {
	llog(logDEBUG) << "I am creating normal flow table ";
	print_timer = NULL;
}

Async_Auto_Flow_Table::Async_Auto_Flow_Table(std::string filename, uint32_t timeout) {

	llog(logDEBUG) << "I am creating printable flow table " << filename;
	std::ofstream fh;
	fh.open(filename, std::ios::out);
	fh << "\n";
	fh.close();

	print_timer = new boost::asio::deadline_timer(table_io, boost::posix_time::milliseconds(timeout));
	print_timer->async_wait(boost::bind(&Async_Auto_Flow_Table::printStatus, this, boost::asio::placeholders::error, filename, timeout));
}

Async_Auto_Flow_Table::~Async_Auto_Flow_Table() {
	llog(logDEBUG) << "Destroy this flow table";
	if (print_timer) {
		print_timer->cancel();
		delete(print_timer);
	}

}

int Async_Auto_Flow_Table::getValue(Flow flow) {
	llog(logDEBUG2) << "get value from flow table " << flow;
	int value = -1;
	table_mutex.lock();
	if (table.count(flow) > 0) {
		value = table[flow];
	}
	table_mutex.unlock();
	return value;
}


/*
 * addValue: adds the value
 *	input: 	flow - some arbitrary flow
 *			value - the value to add and decrement
 *			max - some value to compare the new table value to, give a non positive if no comparison should take place
 *			timeout - the time in ms for the decrement to occur
 *	output: the given value or -1 if the max was surpassed with the new value added and not already surpassed
 */
int Async_Auto_Flow_Table::addValue(Flow flow, int value, int max, uint32_t timeout) {
	int ret = value;
	table_mutex.lock();

	table[flow] += value;

	llog(logDEBUG2) << "adding " << value << " to " << flow << " to make " << table[flow];

	if (table[flow] > max) {
		llog(logDEBUG) << "------------------------------------------------------------";
		llog(logDEBUG) << "FOUND higher value - " << flow;
		llog(logDEBUG) << "------------------------------------------------------------";
	}

	if ((max > 0) && (table[flow] > max) ) {
		if (recent.count(flow) == 0) {
			 recent[flow] = false;
		}
		if(!recent[flow]){
			ret = -1;
			recent[flow] = true;
		}
	}
	table_mutex.unlock();

	boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(table_io, boost::posix_time::milliseconds(timeout)));
	timer->async_wait(boost::bind(&Async_Auto_Flow_Table::decrement, this, boost::asio::placeholders::error, timer, flow, value));
	return ret;
}


void Async_Auto_Flow_Table::decrement(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, Flow flow, int value) {
	timer.reset();

	table_mutex.lock();

	recent[flow] = false;
	table[flow] -= value;

	llog(logDEBUG2) << "removing " << value << " to " << flow << " to make " << table[flow];

	if (table[flow] == 0)
		table.erase(flow);

	table_mutex.unlock();
}

void Async_Auto_Flow_Table::printStatus(const boost::system::error_code& e, std::string filename, uint32_t timeout) {

	if (e == boost::asio::error::operation_aborted) {
		return;
	}

	std::ofstream fh;
	fh.open(filename, std::ios::out | std::ios::app);

	table_mutex.lock();

	for ( auto iter = table.begin(); iter != table.end(); ++ iter ) {
		fh << "," << Helpers::ip_to_string(iter->first.src_ip) << "," << iter->second;
	}
	table_mutex.unlock();

	fh << "\n";
	fh.close();

	print_timer->expires_from_now(boost::posix_time::milliseconds(timeout));
	print_timer->async_wait(boost::bind(&Async_Auto_Flow_Table::printStatus, this, boost::asio::placeholders::error, filename, timeout));

}
