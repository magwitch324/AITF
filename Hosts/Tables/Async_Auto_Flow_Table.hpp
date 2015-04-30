/*
 * Async_Auto_Flow_Table.hpp
 *
 *  Created on: Apr 29, 2015
 *      Author: Sean
 */

#ifndef ASYNC_AUTO_FLOW_TABLE_HPP_
#define ASYNC_AUTO_FLOW_TABLE_HPP_

#include <unordered_map>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>

#include "Timed_Table.hpp"
#include "../Flow.hpp"

class Async_Auto_Flow_Table : public Timed_Table {

	public:
		Async_Auto_Flow_Table();
		Async_Auto_Flow_Table(std::string filename, uint32_t timeout);
		~Async_Auto_Flow_Table();

		int getValue(Flow flow);
		int addValue(Flow flow, int value, int max, uint32_t timeout);

	private:
		void decrement(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, Flow flow, int value);
		void printStatus(const boost::system::error_code& e, std::string filename, uint32_t timeout);

		std::unordered_map<Flow, int, KeyHasher> table;
		boost::asio::deadline_timer * print_timer;

};







#endif /* ASYNC_AUTO_FLOW_TABLE_HPP_ */
