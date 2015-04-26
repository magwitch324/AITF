/*
 * AttackManager.hpp
 */

#ifndef PACKETMANAGER_HPP
#define PACKETMANAGER_HPP

#include <stdlib.h>

#include <libmnl/libmnl.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nfnetlink.h>

#include <linux/types.h>
#include <linux/netfilter/nfnetlink_queue.h>

#include <libnetfilter_queue/libnetfilter_queue.h>

#include "AttackManager.hpp"
#include "VictimManager.hpp"

class PacketManager {
	public:
		PacketManager();

	private:
		~PacketManager();
		struct nfq_handle * my_netfilterqueue_handle;
		AttackManager * attack_manager;
		VictimManager * victim_manager;
};

#endif /* PACKETMANAGER_HPP */
