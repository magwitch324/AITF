#include <boost/bind.hpp>

#include "Udp_Server.hpp"

using namespace boost::asio;
using boost::asio::ip::udp;


Udp_Server::Udp_Server(Packet_Listener* listener_in){
	listener = listener_in;
}

//Starts the udp server.
void Udp_Server::start(){
	log(logINFO) << "Starting udp server";

	//create the socket
	udp::socket test(udp_io, udp::endpoint(udp::v4(), 13));
	udp_sock = &test;

	//start async listening
	udp_listen();

	udp_io.run();

}

//stops the udp server.
void Udp_Server::stop(){
	log(logINFO) << "Stopping udp server";
	udp_io.stop();
	log(logINFO) << "Udp server stopped";
}

//async listens for udp packets
void Udp_Server::udp_listen(){
	udp_sock->async_receive_from(buffer(recv_buff), remote_endpoint_,
			boost::bind(&Udp_Server::handle_receive, this, 
				placeholders::error, placeholders::bytes_transferred));
}

//Called when a udp packet is received
void Udp_Server::handle_receive(const boost::system::error_code& error,
		std::size_t size){
	log(logINFO) << "-------------------Receiving message-------------------";

	//copy out the data
	std::vector<uint8_t> data(recv_buff.begin(), recv_buff.begin() + size);
	//listen for new connections
	udp_listen();

	//pass the packet to the listener
	listener->packet_arrived(data);

}

