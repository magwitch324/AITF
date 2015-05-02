#include <boost/bind.hpp>

#include "Udp_Server.hpp"
#include "../Helpers.hpp"

Udp_Server * Udp_Server::udpserver = 0;

using namespace boost::asio;
using boost::asio::ip::udp;

bool Udp_Server::registerIP(uint32_t ip, PacketManager * pm) {
	llog(logDEBUG2) << "Registering " << Helpers::ip_to_string(ip);
	if(reg_listens.count(ip) > 0) {
		return false;
	}

	reg_listens[ip] = pm;

	return true;
}

Udp_Server * Udp_Server::getInstance() {
	llog(logDEBUG) << "Getting instance of server";
	if(!udpserver){
		udpserver=new Udp_Server();
	}
	return udpserver;
}

Udp_Server::Udp_Server() {
	llog(logINFO) << "Creating server instance";
}

//Starts the udp server.
void Udp_Server::start(){
	llog(logINFO) << "Starting udp server";

	//create the socket
	udp::socket test(udp_io, udp::endpoint(udp::v4(), 50000));
	udp_sock = &test;

	//start async listening
	udp_listen();

	udp_io.run();

}

//stops the udp server.
void Udp_Server::stop(){
	llog(logINFO) << "Stopping udp server";
	udp_io.stop();
	llog(logINFO) << "Udp server stopped";
}

//async listens for udp packets
void Udp_Server::udp_listen(){
	udp_sock->async_receive_from(buffer(recv_buff), remote_endpoint_, boost::bind(&Udp_Server::handle_receive, this, placeholders::error, placeholders::bytes_transferred));
}

//Called when a udp packet is received
void Udp_Server::handle_receive(const boost::system::error_code& error, std::size_t size){
	llog(logINFO) << "-------------------Received UDP message-------------------";

	uint32_t ip = 0;

	//copy out the data
	std::vector<uint8_t> data(recv_buff.begin(), recv_buff.begin() + size);
	//listen for new connections
	udp_listen();

	uint8_t msg_type = data[0];
	llog(logDEBUG) << "Message type: " << (int) msg_type;

	if( msg_type != 4 ) {
		llog(logWARNING) << "Improper message type for host";
		return;
	}

	memcpy(&ip, &data[1], 4);

	//pass the packet to the listener
	if(reg_listens.count(ip) > 0) {

		reg_listens[ip]->aitfCommunication(data);
	}


}

