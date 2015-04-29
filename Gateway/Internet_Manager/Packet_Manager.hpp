/*
 * AttackManager.hpp
 */

#ifndef PACKET_MANAGER_HPP
#define PACKET_MANAGER_HPP

#include <stdlib.h>
#include <boost/atomic.hpp>
#include <libmnl/libmnl.h>
#include <netinet/in.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nfnetlink_queue.h>
#include <libnetfilter_queue/libnetfilter_queue.h>
#include "Packet_Sniffer.hpp"


class Packet_Manager {
	public:
		Packet_Manager(Packet_Sniffer* listener_in);
		~Packet_Manager();
		void start();
		void stop();

	private:
		static int packet_callback(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data);
		static uint32_t print_pkt (struct nfq_data *tb);
		enum {STARTING, STARTED, ENDING, ENDED};
		mutable boost::atomic<int> state;
		static Packet_Sniffer* listener;
		struct nfq_handle * my_netfilterqueue_handle;
		struct nfq_q_handle * internet_queue_handle;

};

#endif
