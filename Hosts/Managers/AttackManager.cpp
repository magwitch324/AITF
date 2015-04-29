/*
 * ExitManager.cpp
 */

#include "AttackManager.hpp"
#include "../logger.hpp"

#include <boost/function.hpp>
#include <boost/bind.hpp>

static u_int32_t print_pkt (struct nfq_data *tb) {
	int id = 0;
	struct nfqnl_msg_packet_hdr *ph;

	ph = nfq_get_msg_packet_hdr(tb);
	if (ph) {
		id = ntohl(ph->packet_id);
	}

	return id;
}

int attack_callback(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data) {
	llog(logINFO) << "Attack Received Packet";
	u_int32_t id = print_pkt(nfad);
	return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
}

AttackManager::AttackManager(struct nfq_handle * a_nfq_handle, int attack_queue_num) {
	llog(logINFO) << "Starting AttackManager";

	this->state.store(this->STARTING, boost::memory_order_relaxed);

	this->attack_queue_handle = nfq_create_queue(a_nfq_handle, attack_queue_num, &attack_callback, NULL);

	nfq_set_mode(this->attack_queue_handle, NFQNL_COPY_PACKET, 0xffff);

	this->packet_thread = new boost::thread(boost::bind(&AttackManager::packetThreadFunc, this, _1), a_nfq_handle);

	this->state.store(this->STARTED, boost::memory_order_relaxed);

}

AttackManager::~AttackManager(void) {
	llog(logINFO) << "Ending AttackManager";

	this->state.store(this->ENDING, boost::memory_order_relaxed);
	this->packet_thread->interrupt();
	this->packet_thread->join();
	nfq_destroy_queue( this->attack_queue_handle );
}


void AttackManager::packetThreadFunc(struct nfq_handle * a_nfq_handle) {
	llog(logINFO) << "Starting Attack Thread";
	int fd;
	int rv;
	char buf[4096] __attribute__ ((aligned));

	fd_set readset;
	struct timeval tv;

	fd = nfq_fd(a_nfq_handle);

	while (this->ENDING != this->state.load(boost::memory_order_relaxed)) {
		FD_ZERO(&readset);
		FD_SET(fd, &readset);
		tv.tv_sec = 0;
		tv.tv_usec = 500;
		if (select(fd+1, &readset, NULL, NULL, &tv) <= 0 ) {
			continue;
		}

		rv = recv(fd, buf, sizeof(buf), 0);//MSG_DONTWAIT );
		if (rv < 0) {
			continue;
		}
		llog(logINFO) << "Attack Queue is Receiceiving Packet";
		nfq_handle_packet(a_nfq_handle, buf, rv);
	}
}
