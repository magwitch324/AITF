#ifndef FILTER_SET_HPP
#define FILTER_SET_HPP

#include <unordered_map>
#include <vector>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "../Flow.hpp"
#include "../Constants.hpp"





class Filter_Set{
	public:
		Filter_Set();
		Filter_Set(boost::mutex* mutex_, boost::asio::io_service* table_io_);
		void add_temp_filter(Flow flow);
		void add_long_filter(Flow flow);
		void add_gtw_rvalue(uint32_t gtw_ip, uint64_t rvalue);
		int attempt_count(Flow flow);
		bool flow_is_filtered(Flow flow);

	private:
		void add_filter(Flow, int secs);
		void decrement_flow_filter(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, int secs, boost::shared_ptr<Flow> flow);
		void decrement_gateway_filter(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, int secs, uint32_t gtw_ip);
		std::unordered_map<Flow, int, KeyHasher> flow_filters;
		std::unordered_map<uint32_t, int> gateway_filters;
		std::unordered_map<uint32_t, uint64_t> rvalue_filters;
		boost::mutex* table_mutex;
		boost::asio::io_service* table_io;
};

#endif
