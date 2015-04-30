#include "HostManager.hpp"
#include "../logger.hpp"


#include <boost/function.hpp>
#include <boost/bind.hpp>

u_int32_t HostManager::getPktID (struct nfq_data *tb) {
	int id = 0;
	struct nfqnl_msg_packet_hdr *ph;

	ph = nfq_get_msg_packet_hdr(tb);
	if (ph) {
		id = ntohl(ph->packet_id);
	}

	return id;
}

HostManager::HostManager(struct nfq_handle * a_nfq_handle, int queue_num) {

	llog(logINFO) << "Starting a host manager";
	state.store(STARTING, boost::memory_order_relaxed);

	queue_handle = nfq_create_queue(a_nfq_handle, queue_num, &callbackForward, this);

	nfq_set_mode(queue_handle, NFQNL_COPY_PACKET, 0xffff);

	packet_thread = new boost::thread(boost::bind(&HostManager::packetRecieveThreadFunc, this, _1), a_nfq_handle);

	state.store(STARTED, boost::memory_order_relaxed);

}


void HostManager::packetRecieveThreadFunc(struct nfq_handle * a_nfq_handle) {
	llog(logINFO) << "a Host Manager Received a packet";
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

		rv = recv(fd, buf, sizeof(buf), 0);
		if (rv < 0) {
			continue;
		}

        nfq_handle_packet(a_nfq_handle, buf, rv);
	}
}
