/*
 * ExitManager.cpp
 */

#include <netinet/ip.h>
#include <arpa/inet.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "AttackManager.hpp"
#include "../logger.hpp"
#include "../Helpers.hpp"

AttackManager::AttackManager(uint32_t ip, uint32_t gtw, struct nfq_handle * a_nfq_handle, int attack_queue_num, FilterModule * fil) : HostManager(a_nfq_handle, attack_queue_num)  {
	llog(logINFO) << "Starting AttackManager - " << ip;
	filter = fil;
	my_ip = ip;
	my_gtw = gtw;
}

AttackManager::~AttackManager(void)  {
	llog(logINFO) << "Destroying AttackManager - " << my_ip;
	state.store(this->ENDING, boost::memory_order_relaxed);
	packet_thread->interrupt();
	packet_thread->join();
	nfq_destroy_queue( queue_handle );
}

int AttackManager::packetCallbackFunc(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data) {
	unsigned char * buffer;
	uint32_t destination_ip = 0;

	llog(logINFO) << "Host is sending a Packet";
	u_int32_t id = getPktID(nfad);

	nfq_get_payload(nfad, &buffer);
	destination_ip = *((uint32_t *) (buffer + 16));
	
	if( filter->shouldFilter(destination_ip) > 0 ) {
		llog(logINFO) << "Host is filtering towards " << Helpers::ip_to_string(destination_ip);
		return nfq_set_verdict(qh, id, NF_DROP, 0, NULL);
	} 

	llog(logINFO) << "Host is allowing traffic to " << Helpers::ip_to_string(destination_ip);
	return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);

}

void AttackManager::addFilter(uint32_t dest, int timeout) {
	llog(logINFO) << "Attack got request to add filter Packet";

	if(filter->shouldFilter(dest)) {
		llog(logERROR) << " --- Attack Received a filter request for a location that it already filters";
	}

	filter->addNewFilter(dest, timeout);
	this->sendFilterResponse(dest);
}


void AttackManager::sendFilterResponse(uint32_t dest) {
	llog(logINFO) << "Attack is sending reply to attack";
	using namespace boost::asio;
	using boost::asio::ip::udp;

	boost::asio::io_service io_service;
	udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(), Helpers::ip_to_string(my_gtw), "50000");
	udp::endpoint receiver_endpoint = *resolver.resolve(query);

	udp::socket socket(io_service);
	socket.open(udp::v4());

	std::vector<uint8_t> message(9);

	message[0] = 5;
	memcpy(&message[1], &my_ip, 4);
	memcpy(&message[5], &dest, 4);

	llog(logERROR) << "The dest here is: " << Helpers::ip_to_string(dest);

	llog(logERROR) << "The src here is: " << Helpers::ip_to_string(my_ip);

	socket.send_to(boost::asio::buffer(message), receiver_endpoint);
}
