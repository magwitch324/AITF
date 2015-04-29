/*
 * VictimManager.cpp
 */


#include "VictimManager.hpp"
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

int victim_callback(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data) {
	llog(logINFO) << "Victim Received Packet";
	u_int32_t id = print_pkt(nfad);
	return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
}

VictimManager::VictimManager(struct nfq_handle * a_nfq_handle, int victim_queue_num) {
	llog(logINFO) << "Starting VictimManager";

	this->state.store(this->STARTING, boost::memory_order_relaxed);

	this->victim_queue_handle = nfq_create_queue(a_nfq_handle, victim_queue_num, &victim_callback, NULL);

	nfq_set_mode(this->victim_queue_handle, NFQNL_COPY_PACKET, 0xffff);

	this->packet_thread = new boost::thread(boost::bind(&VictimManager::packetThreadFunc, this, _1), a_nfq_handle);

	this->state.store(this->STARTED, boost::memory_order_relaxed);

}

VictimManager::~VictimManager(void) {
	llog(logINFO) << "Victim Ending VictimManager";

	this->state.store(this->ENDING, boost::memory_order_relaxed);
	this->packet_thread->interrupt();
	this->packet_thread->join();
	nfq_destroy_queue( this->victim_queue_handle );
}


void VictimManager::packetThreadFunc(struct nfq_handle * a_nfq_handle) {
	llog(logINFO) << "Starting Victim Thread";
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
		llog(logINFO) << "Victim Queue is Receiceiving Packet";
		nfq_handle_packet(a_nfq_handle, buf, rv);
	}
}
