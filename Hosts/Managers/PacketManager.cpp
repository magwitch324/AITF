
#include "PacketManager.hpp"
#include "../logger.hpp"
#include "../Servers/Udp_Server.hpp"

PacketManager::PacketManager(uint32_t a_ip, int input_queue_num, int output_queue_num, PolicyModule * policy, FilterModule * filter) {
	llog(logINFO) << "Starting PacketManager";

	ip = a_ip;

	this->my_netfilterqueue_handle = nfq_open();
	nfq_unbind_pf(this->my_netfilterqueue_handle, AF_INET);
	nfq_bind_pf(this->my_netfilterqueue_handle, AF_INET);

	this->victim_manager = new VictimManager(this->my_netfilterqueue_handle, input_queue_num, policy);
	this->attack_manager = new AttackManager(this->my_netfilterqueue_handle, output_queue_num, filter);

	Udp_Server * udps;
	udps = udps->getInstance();
	udps->registerIP(ip, this);
}


PacketManager::~PacketManager(void) {
	llog(logINFO) << "Ending PacketManger";
	delete(this->attack_manager);
	delete(this->victim_manager);
	nfq_close(this->my_netfilterqueue_handle);
	llog(logINFO) << "Ended PacketManger";
}

void PacketManager::aitfCommunication(std::vector<uint8_t> recv_buf) {
	log(logDEBUG2) << "Checking message type";

		//extract the type of message
	uint8_t msg_type = recv_buf[0];
	log(logDEBUG) << "Message type: " << (int) msg_type;

	switch(msg_type){
		case 0: //Filter request
			//We should never receive one of these
			break;
		case 1: //Handshake request
			//We should never receive one of these
			break;
		case 2: //Handshake Part 2
			//We should never receive one of these
			break;
		case 3: //Handshake part 3
			//We should never receive one of these
			break;
		case 4: //Attack filter request
			//TODO: Add to filter and send response
			break;
		case 5: //Filter request reply
			//We should never receive one of these
			break;
		default:
			log(logWARNING) << "Invalid message type";
			break;
	}
}
