#ifndef FILTER_SET_HPP
#define FILTER_SET_HPP

#include <unordered_map>
#include <vector>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
#include "../Flow.hpp"

#define TEMP_TIME 1
#define LONG_TIME 10



class Filter_Set{
	public:
		Filter_Set();
		Filter_Set(boost::mutex* mutex_, boost::asio::io_service* table_io_);
		void add_temp_filter(Flow flow);
		void add_long_filter(Flow flow);
		bool is_flow_filtered(Flow flow);

	private:
		void add_filter(Flow, int secs);
		void decrement_flow_filter(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, int secs, Flow* flow);
		void decrement_gateway_filter(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, int secs, uint32_t gtw_ip);
		std::unordered_map<Flow, int, KeyHasher> flow_filters;
		std::unordered_map<uint32_t, int> gateway_filters;
		boost::mutex* table_mutex;
		boost::asio::io_service* table_io;
};

#endif
