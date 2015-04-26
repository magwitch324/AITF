/*
 * VictimManager.hpp
 *
 *  Created on: Apr 24, 2015
 *      Author: Sean
 */

#ifndef VICTIMMANAGER_HPP_
#define VICTIMMANAGER_HPP_

#include <stdlib.h>

#include <libmnl/libmnl.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nfnetlink.h>

#include <linux/types.h>
#include <linux/netfilter/nfnetlink_queue.h>

#include <libnetfilter_queue/libnetfilter_queue.h>

class VictimManager {
	public:
		VictimManager(nfq_handle * a_nfq_handle);

	private:
		~VictimManager();
		nfq_q_handle * victim_queue_handle;

};


#endif /* VICTIMMANAGER_HPP_ */
