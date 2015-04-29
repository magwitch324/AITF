#include "HostManager.hpp"
#include "../logger.hpp"


#include <boost/function.hpp>
#include <boost/bind.hpp>

static u_int32_t HostManager::getPktID (struct nfq_data *tb) {
	int id = 0;
	struct nfqnl_msg_packet_hdr *ph;
	struct nfqnl_msg_packet_hw *hwph;
	u_int32_t mark,ifi;
	int ret;
	unsigned char *data;

	ph = nfq_get_msg_packet_hdr(tb);
	if (ph) {
	        id = ntohl(ph->packet_id);
	        printf("hw_protocol=0x%04x hook=%u id=%u ",
	                ntohs(ph->hw_protocol), ph->hook, id);
	}

	hwph = nfq_get_packet_hw(tb);
	if (hwph) {
	        int i, hlen = ntohs(hwph->hw_addrlen);

	        printf("hw_src_addr=");
	        for (i = 0; i < hlen-1; i++)
	                printf("%02x:", hwph->hw_addr[i]);
	        printf("%02x ", hwph->hw_addr[hlen-1]);
	}

	mark = nfq_get_nfmark(tb);
	if (mark)
	        printf("mark=%u ", mark);

	ifi = nfq_get_indev(tb);
	if (ifi)
	        printf("indev=%u ", ifi);

	ifi = nfq_get_outdev(tb);
	if (ifi)
	        printf("outdev=%u ", ifi);
	ifi = nfq_get_physindev(tb);
	if (ifi)
	        printf("physindev=%u ", ifi);

	ifi = nfq_get_physoutdev(tb);
	if (ifi)
	        printf("physoutdev=%u ", ifi);

	ret = nfq_get_payload(tb, &data);
	if (ret >= 0)
	        printf("payload_len=%d ", ret);

	fputc('\n', stdout);

	return id;

	/*int id = 0;
	struct nfqnl_msg_packet_hdr *ph;

	ph = nfq_get_msg_packet_hdr(tb);
	if (ph) {
		id = ntohl(ph->packet_id);
	}

	return id;*/
}

HostManager::HostManager(struct nfq_handle * a_nfq_handle, int queue_num) {

	state.store(STARTING, boost::memory_order_relaxed);

	queue_handle = nfq_create_queue(a_nfq_handle, queue_num, &callbackForward, this);

	nfq_set_mode(queue_handle, NFQNL_COPY_PACKET, 0xffff);

	packet_thread = new boost::thread(boost::bind(&HostManager::packetRecieveThreadFunc, this, _1), a_nfq_handle);

	state.store(STARTED, boost::memory_order_relaxed);

}


void HostManager::packetRecieveThreadFunc(struct nfq_handle * a_nfq_handle) {
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
