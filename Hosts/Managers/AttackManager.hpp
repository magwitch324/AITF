/*
 * AttackManager.hpp
 */

#ifndef ATTACKMANAGER_HPP
#define ATTACKMANAGER_HPP

#include <stdlib.h>

#include <libmnl/libmnl.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nfnetlink.h>

#include <linux/types.h>
#include <linux/netfilter/nfnetlink_queue.h>

#include <libnetfilter_queue/libnetfilter_queue.h>

class AttackManager {
	public:
		AttackManager(nfq_handle * a_nfq_handle);

	private:
		~AttackManager();
		nfq_q_handle * attack_queue_handle;
};

#endif /* ATTACKMANAGER_HPP_ */
