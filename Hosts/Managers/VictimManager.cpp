/*
 * VictimManager.cpp
 */

#include "VictimManager.hpp"
#include "../logger.hpp"

static u_int32_t print_pkt (struct nfq_data *tb) {
	int id = 0;
	struct nfqnl_msg_packet_hdr *ph;

	ph = nfq_get_msg_packet_hdr(tb);
	if (ph) {
		id = ntohl(ph->packet_id);
	}

	return id;
}

static int attack_callback(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data) {
	u_int32_t id = print_pkt(nfad);
	return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
}

VictimManager::VictimManager(nfq_handle * a_nfq_handle) {
	llog(logINFO) << "Starting VictimManager";
	//this->attack_queue_handle = nfq_create_queue(a_nfq_handle, 10, &attack_callback, this);
}

VictimManager::~VictimManager(void) {
	llog(logINFO) << "Ending VictimManager";
	//nfq_destroy_queue( this->attack_queue_handle );
}
