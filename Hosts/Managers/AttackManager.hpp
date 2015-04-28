/*
 * AttackManager.hpp
 */

#ifndef ATTACKMANAGER_HPP
#define ATTACKMANAGER_HPP

//#include <stdlib.h>
#include <libmnl/libmnl.h>
#include <netinet/in.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nfnetlink.h>

#include <linux/types.h>
#include <linux/netfilter/nfnetlink_queue.h>

#include <libnetfilter_queue/libnetfilter_queue.h>

#include <boost/atomic.hpp>
#include <boost/thread.hpp>

class AttackManager {
	public:
		AttackManager(struct nfq_handle * a_nfq_handle, int attack_queue_num);
		~AttackManager();

	private:
		enum {STARTING, STARTED, ENDING, ENDED};
		mutable boost::atomic<int> state;
		boost::thread * packet_thread;
		nfq_q_handle * attack_queue_handle;
		void packetThreadFunc(struct nfq_handle * a_nfq_handle);

};

#endif /* ATTACKMANAGER_HPP_ */
