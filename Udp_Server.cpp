#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "Udp_Server.hpp"

using namespace boost::asio;
using boost::asio::ip::udp;


Udp_Server::Udp_Server(Packet_Listener* listener_in){
	listener = listener_in;
}

void Udp_Server::start(){
	io_service udp_io;

	udp::socket udp_sock(udp_io, udp::endpoint(udp::v4(), 13));

	udp::endpoint remote_endpoint_;

	udp_sock.async_receive_from(buffer(recv_buff), remote_endpoint_,
        boost::bind(&Udp_Server::handle_receive, this, 
        	placeholders::error, placeholders::bytes_transferred));


	udp_io.run();

}

void Udp_Server::stop(){

}

void Udp_Server::handle_receive(const boost::system::error_code& error,
      std::size_t){

}

