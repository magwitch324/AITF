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
#include "../Helpers.hpp"

VictimManager::VictimManager(uint32_t ip, uint32_t gtw, struct nfq_handle * a_nfq_handle, int victim_queue_num, PolicyModule * pol) : HostManager(a_nfq_handle, victim_queue_num) {
	llog(logINFO) << "Starting VictimManager";
	policy = pol;
	my_ip = ip;
	my_gtw = gtw;
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
	llog(logINFO) << "Computing checksum";
	iphdrp->check = 0;
	iphdrp->check = compute_checksum((unsigned short*)iphdrp, iphdrp->ihl<<2);
}


int VictimManager::packetCallbackFunc(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data) {
	llog(logINFO) << "Victim Received Packet";

	struct nfqnl_msg_packet_hdr *ph;
	int len;
	unsigned char *ORIGINAL_DATA;
	struct iphdr *ipHeader;

	//get the packet id
	ph = nfq_get_msg_packet_hdr(nfad);
	u_int32_t id = ntohl(ph->packet_id);

	//get the packet contents
	len = nfq_get_payload(nfad, &ORIGINAL_DATA);

	ipHeader = (struct iphdr *)ORIGINAL_DATA;

	if(ipHeader->protocol == 143){
		llog(logDEBUG) << "Received AITF PACKET!!!!";
		unsigned char modified_packet[len];
		memcpy(&modified_packet[0], ORIGINAL_DATA, len);
		Flow flow(std::vector<uint8_t>(&modified_packet[sizeof(*ipHeader)+1], &modified_packet[sizeof(*ipHeader)+1]+81));

		for(int i = 0; i <= flow.pointer; i++){
			uint32_t ipInt = flow.get_gtw_ip_at(i);
			in_addr* addr = (in_addr*) &ipInt;
			llog(logERROR) << "gtw ip " << i << inet_ntoa(*addr);
		}
		
		int val = policy->receivedPacket(flow, len-82);
		llog(logERROR) << "RETURN VALUE: " << val;

		if(val < 0) {
			this->SendFilterRequest(&flow);
		}

		llog(logDEBUG) << "Destination is this gateway";
		unsigned char actual_packet[len-82];
		memcpy(&actual_packet[0], ORIGINAL_DATA, sizeof(*ipHeader));
		memcpy(&actual_packet[sizeof(*ipHeader)], ORIGINAL_DATA + sizeof(*ipHeader) + 82, len - sizeof(*ipHeader) - 82);
		((struct iphdr*) &actual_packet[0])->protocol = *((uint8_t*)ORIGINAL_DATA+sizeof(*ipHeader));
		((struct iphdr*) &actual_packet[0])->tot_len = htons(len-82);

		//compute the new checksum
		compute_ip_checksum((struct iphdr*) &actual_packet[0]);
		llog(logDEBUG2) << std::hex << ((struct iphdr*) &actual_packet[0])->check;
		return nfq_set_verdict(qh, id, NF_ACCEPT, len-82, &actual_packet[0]);

	}

	llog(logDEBUG) << "Received Normal PACKET :(";
	return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);

}

void VictimManager::SendFilterRequest(Flow * flow) {
	llog(logDEBUG) << "Sending Filter request - gtwy r" << flow->gtw0_rvalue;
	using namespace boost::asio;
	using boost::asio::ip::udp;

	boost::asio::io_service io_service;
	udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(),  Helpers::ip_to_string(my_gtw), "50000");
	udp::endpoint receiver_endpoint = *resolver.resolve(query);

	udp::socket socket(io_service);
	socket.open(udp::v4());

	std::vector<uint8_t> message(83);

	message[0] = 0;
	memcpy(&message[1], &flow->to_byte_vector()[0], 81);
	message[82] = 0;

	socket.send_to(boost::asio::buffer(message), receiver_endpoint);
}
