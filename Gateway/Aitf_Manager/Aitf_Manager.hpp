#ifndef AITF_MANAGER_HPP
#define AITF_MANAGER_HPP
#include "Packet_Listener.hpp"
#include "Udp_Server.hpp"
#include "../Tables/Aitf_Hosts_Table.hpp"
#include "../Tables/Filter_Table.hpp"

class Aitf_Manager : public Packet_Listener
{
	public:
		Aitf_Manager();
		void start_thread();
		void stop_thread();
		virtual void packet_arrived(std::vector<uint8_t> recv_buf);

	private:
		void run();
		void timeout_run();
		void handle_request(std::vector<uint8_t> message);
		void unresponsive_host(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, uint8_t flow[]);
		boost::thread aitf_thread;
		boost::thread timeout_thread;
		boost::asio::io_service timeout_io;
		boost::shared_ptr<boost::asio::io_service::work> timeout_work;
		Udp_Server* server;

};

#endif
