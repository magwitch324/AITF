/*
 * ExitManager.cpp
 */

#include "AttackManager.hpp"
#include "Managers/PacketManager.hpp"

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
	log(logINFO) << "Received Packet";
	u_int32_t id = print_pkt(nfa);
	return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
}

AttackManager::AttackManager(nfq_handle * a_nfq_handle) {
	log(logINFO) << "Starting AttackManager";
	this->attack_queue_handle = nfq_create_queue(a_nfq_handle, 10, &attack_callback, this);
}

AttackManager::~AttackManager(void) {
	log(logINFO) << "Ending AttackManager";
	nfq_destroy_queue( this->attack_queue_handle );
}
