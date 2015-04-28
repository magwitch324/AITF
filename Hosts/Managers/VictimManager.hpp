/*
 * VictimManager.hpp
 *
 *  Created on: Apr 24, 2015
 *      Author: Sean
 */

#ifndef VICTIMMANAGER_HPP_
#define VICTIMMANAGER_HPP_

#include <libmnl/libmnl.h>
#include <netinet/in.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nfnetlink.h>

#include <linux/types.h>
#include <linux/netfilter/nfnetlink_queue.h>

#include <libnetfilter_queue/libnetfilter_queue.h>

#include <boost/atomic.hpp>
#include <boost/thread.hpp>

class VictimManager {
	public:
		VictimManager(struct nfq_handle * a_nfq_handle, int victim_queue_num);
		~VictimManager();

	private:
		enum {STARTING, STARTED, ENDING, ENDED};
		mutable boost::atomic<int> state;
		boost::thread * packet_thread;
		nfq_q_handle * victim_queue_handle;
		void packetThreadFunc(struct nfq_handle * a_nfq_handle);

};


#endif /* VICTIMMANAGER_HPP_ */
