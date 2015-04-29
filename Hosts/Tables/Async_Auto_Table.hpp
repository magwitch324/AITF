#ifndef ASYNC_AUTO_TABLE_HPP
#define ASYNC_AUTO_TABLE_HPP

#include <unordered_map>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>

#include "Timed_Table.hpp"

class Async_Auto_Table : public Timed_Table {

	public:
		Async_Auto_Table();
		Async_Auto_Table(std::string filename, uint32_t timeout);
		~Async_Auto_Table();

		int getValue(uint32_t ip);
		int addValue(uint32_t ip, int value, int max, uint32_t timeout);
		int compareValue(uint32_t ip, int value);

	private:
		void decrement(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, uint32_t ip, int value);
		void printStatus(const boost::system::error_code& e, std::string filename, uint32_t timeout);

		std::unordered_map<uint32_t, int> table;
		boost::asio::deadline_timer * print_timer;
		//std::ofstream filehandle;
};

#endif
