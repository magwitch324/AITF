/*
 * VictimManager.cpp
 */

#include "VictimManager.hpp"
#include "Managers/PacketManager.hpp"

static int attack_callback(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data) {
	u_int32_t id = print_pkt(nfa);
	printf("entering callback\n");
	return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
}

VictimManager::VictimManager(nfq_handle * a_nfq_handle) {
	log(logINFO) << "Starting VictimManager";
	//this->attack_queue_handle = nfq_create_queue(a_nfq_handle, 10, &attack_callback, this);
}

VictimManager::~VictimManager(void) {
	log(logINFO) << "Ending VictimManager";
	//nfq_destroy_queue( this->attack_queue_handle );
}
