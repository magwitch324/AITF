#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "Packet_Listener.hpp"
#include "../logger.hpp"


class Udp_Server{

	public:
		Udp_Server(Packet_Listener* listener_in);
		Udp_Server();

		void start();
		void stop();

	private:
		void handle_receive(const boost::system::error_code& error, std::size_t);
		void udp_listen();
		Packet_Listener* listener;
		boost::asio::ip::udp::socket* udp_sock = NULL;
		boost::asio::ip::udp::endpoint remote_endpoint_;
		boost::array<uint8_t, 165> recv_buff;
		boost::asio::io_service udp_io;

};

#endif