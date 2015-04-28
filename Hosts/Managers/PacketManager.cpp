
#include "PacketManager.hpp"
#include "../logger.hpp"

PacketManager::PacketManager(int input_queue_num, int output_queue_num) {
	llog(logINFO) << "Starting PacketManager";

	this->my_netfilterqueue_handle = nfq_open();
	nfq_unbind_pf(this->my_netfilterqueue_handle, AF_INET);
	nfq_bind_pf(this->my_netfilterqueue_handle, AF_INET);

	this->victim_manager = new VictimManager(this->my_netfilterqueue_handle, input_queue_num);
	this->attack_manager = new AttackManager(this->my_netfilterqueue_handle, output_queue_num);
}


PacketManager::~PacketManager(void) {
	llog(logINFO) << "Ending PacketManger";
	delete(this->attack_manager);
	delete(this->victim_manager);
	nfq_close(this->my_netfilterqueue_handle);
	llog(logINFO) << "Ended PacketManger";
}
