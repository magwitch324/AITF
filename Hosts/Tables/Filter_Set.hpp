#ifndef FILTER_SET_HPP
#define FILTER_SET_HPP

#include <unordered_map>
#include <vector>
#include <boost/functional/hash.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>

#define TEMP_TIME 1
#define LONG_TIME 10

class Flow {
	public:
		Flow(uint8_t flow[]);
		std::vector<uint8_t> flow;

		bool operator==(const Flow &other) const
		{ 
			bool equal = true;
			for(int i = 0; i < 81; i++){
				if(flow[i] != other.flow[i]){
					equal = false;
					break;
				}
			}

			return equal;
		}

};

struct KeyHasher{
	std::size_t operator() (const Flow& flow) const{
		return boost::hash_range(flow.flow.begin(), flow.flow.end());
	}
};

class Filter_Set{
	public:
		Filter_Set(boost::mutex* mutex_, boost::asio::io_service* table_io_);
		void add_temp_filter(uint8_t flow[]);
		void add_long_filter(uint8_t flow[]);

	private:
		void add_filter(uint8_t flow[], int secs);
		void decrement_flow_filter(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, int secs, Flow* flow);
		void decrement_gateway_filter(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, int secs, uint32_t gtw_ip);
		std::unordered_map<Flow, int, KeyHasher> flow_filters;
		std::unordered_map<uint32_t, int> gateway_filters;
		boost::mutex* table_mutex;
		boost::asio::io_service* table_io;
};

#endif
