
#include "PacketManager.hpp"
#include "../logger.hpp"

PacketManager::PacketManager(void) {
	llog(logINFO) << "Starting PacketManager";

	this->my_netfilterqueue_handle = nfq_open();
	nfq_unbind_pf(this->my_netfilterqueue_handle, AF_INET);
	nfq_bind_pf(this->my_netfilterqueue_handle, AF_INET);

	this->victim_manager = new VictimManager(this->my_netfilterqueue_handle);
	this->attack_manager = new AttackManager(this->my_netfilterqueue_handle);
}


PacketManager::~PacketManager(void) {
	llog(logINFO) << "Ending PacketManger";
	delete(this->victim_manager);
	delete(this->attack_manager);
	nfq_close(this->my_netfilterqueue_handle);
	llog(logINFO) << "Ended PacketManger";
}
