#ifndef INTERNET_MANAGER_HPP
#define INTERNET_MANAGER_HPP
#include <boost/thread.hpp>
#include "../Tables/Filter_Table.hpp"
#include "Packet_Sniffer.hpp"
#include "Packet_Manager.hpp"

class Internet_Manager : Packet_Sniffer{
	public:
		Internet_Manager();
		~Internet_Manager();
		void packet_callback(std::vector<uint8_t> packet);
		void start_thread();
		void stop_thread();

	private:
		void run();
		void handle_handshake(std::vector<uint8_t> message);
		Packet_Manager* packet_manager;
		boost::thread internet_thread;
};

#endif
