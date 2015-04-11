#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include "Packet_Listener.hpp"


class Udp_Server{

	public:
		Udp_Server(Packet_Listener* listener_in);
		Udp_Server();

		void start();
		void stop();

	private:
		void handle_receive(const boost::system::error_code& error,
      std::size_t);
		Packet_Listener* listener;
		std::vector<char> recv_buff;

};

#endif