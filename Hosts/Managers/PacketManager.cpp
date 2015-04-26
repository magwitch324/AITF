
#include "PacketManager.hpp"
#include "Managers/PacketManager.hpp"

PacketManager::PacketManager(void) {
	log(logINFO) << "Starting PacketManager";

	this->my_netfilterqueue_handle = nfq_open();
	nfq_unbind_pf(my_netfilterqueue_handle, AF_INET);
	nfq_bind_pf(h, AF_INET);

	this->victim_manager = new VictimManager(this->my_netfilterqueue_handle);
	this->attack_manager = new AttackManager(this->my_netfilterqueue_handle);

	qh = nfq_create_queue(h,  0, &cb, NULL);

}


PacketManager::~PacketManager(void) {
	delete this->victim_manager;
	delete this->attack_manager;
	nfq_destroy_queue(my_netfilterqueue_handle);
	log(logINFO) << "Ending PacketManger";
}
