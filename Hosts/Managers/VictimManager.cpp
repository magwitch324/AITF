/*
 * VictimManager.cpp
 */


#include "VictimManager.hpp"
#include "../logger.hpp"


#include <boost/function.hpp>
#include <boost/bind.hpp>

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


int VictimManager::packetCallbackFunc(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data) {
	llog(logINFO) << "Victim Received Packet";
	u_int32_t id = getPktID(nfad);
	return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
}
