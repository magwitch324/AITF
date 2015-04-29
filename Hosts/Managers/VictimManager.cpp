/*
 * VictimManager.cpp
 */


#include <netinet/ip.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "VictimManager.hpp"
#include "../logger.hpp"

VictimManager::VictimManager(struct nfq_handle * a_nfq_handle, int victim_queue_num, PolicyModule * pol) : HostManager(a_nfq_handle, victim_queue_num) {
	llog(logINFO) << "Starting VictimManager";
	policy = pol;
}

VictimManager::~VictimManager(void) {
	llog(logINFO) << "Ending VictimManager";

	this->state.store(this->ENDING, boost::memory_order_relaxed);
	this->packet_thread->interrupt();
	this->packet_thread->join();
	nfq_destroy_queue( queue_handle );
}

static unsigned short compute_checksum(unsigned short *addr, unsigned int count) {
	register unsigned long sum = 0;
	while (count > 1) {
		sum += * addr++;
		count -= 2;
	}
	//if any bytes left, pad the bytes and add
	if(count > 0) {
		sum += ((*addr)&htons(0xFF00));
	}
	//Fold sum to 16 bits: add carrier to result
	while (sum>>16) {
		sum = (sum & 0xffff) + (sum >> 16);
	}
	//one's complement
	sum = ~sum;
	return ((unsigned short)sum);
}

/* set ip checksum of a given ip header*/
void compute_ip_checksum(struct iphdr* iphdrp){
	iphdrp->check = 0;
	iphdrp->check = compute_checksum((unsigned short*)iphdrp, iphdrp->ihl<<2);
}


int VictimManager::packetCallbackFunc(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data) {
	llog(logINFO) << "Victim Received Packet";

	u_int32_t id = getPktID(nfad);

	struct nfqnl_msg_packet_hdr *ph;
	int len;
	unsigned char *ORIGINAL_DATA;
	struct iphdr *ipHeader;

	//get the packet id
	ph = nfq_get_msg_packet_hdr(nfad);
	u_int32_t id = ntohl(ph->packet_id);

	//get the packet contents
	len = nfq_get_payload(nfad, &ORIGINAL_DATA);

	if(ipHeader->protocol == 143){
		log(logDEBUG) << "Received AITF PACKET!!!!";
		unsigned char modified_packet[len];
		memcpy(&modified_packet[0], ORIGINAL_DATA, len);
		Flow flow(std::vector<uint8_t>(&modified_packet[sizeof(*ipHeader)+1], &modified_packet[sizeof(*ipHeader)+1]+81));
		int length = 0;

		int val = policy->receivedPacket(flow, length);

		if (val == -1) {
			this->SendFilterRequest(&flow, false);
		} else if (val == -2) {
			this->SendFilterRequest(&flow, true);
		}

		//if the packet is allowed on then reinsert the new flow and recompute checksum
		memcpy(&modified_packet[sizeof(*ipHeader) + 1], &flow.to_byte_vector()[0], 82);
		//compute the new checksum
		compute_ip_checksum((struct iphdr*) &modified_packet[0]);

		return nfq_set_verdict(qh, id, NF_ACCEPT, len, &modified_packet[0]);
	} else {

		return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);

	}
}

void VictimManager::SendFilterRequest(Flow * flow, bool do_esc) {
	using namespace boost::asio;
	using boost::asio::ip::udp;

	boost::asio::io_service io_service;
	udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(),"10.4.13.4","50000"); //TODO: change query string
	udp::endpoint receiver_endpoint = *resolver.resolve(query);

	udp::socket socket(io_service);
	socket.open(udp::v4());

	std::vector<uint8_t> message(0);

	message[0] = 0;
	memcpy(&message[1], &flow->to_byte_vector()[0], 81);
	message[82] = do_esc;

	socket.send_to(boost::asio::buffer(message), receiver_endpoint);
}
