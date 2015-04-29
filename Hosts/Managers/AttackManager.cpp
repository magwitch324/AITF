/*
 * ExitManager.cpp
 */

#include "AttackManager.hpp"
#include "../logger.hpp"

#include <boost/function.hpp>
#include <boost/bind.hpp>

AttackManager::AttackManager(struct nfq_handle * a_nfq_handle, int attack_queue_num, FilterModule * fil) : HostManager(a_nfq_handle, attack_queue_num)  {
	llog(logINFO) << "Starting AttackManager";
	filter = fil;
}

AttackManager::~AttackManager(void) {
	state.store(this->ENDING, boost::memory_order_relaxed);
	packet_thread->interrupt();
	packet_thread->join();
	nfq_destroy_queue( queue_handle );
}

int VictimManager::packetCallbackFunc(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data) {
	llog(logINFO) << "Attack Received Packet";
	u_int32_t id = getPktID(nfad);

	return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
}
