/*
 * AttackManager.hpp
 */

#ifndef PACKETMANAGER_HPP
#define PACKETMANAGER_HPP

#include <stdlib.h>

#include <libmnl/libmnl.h>
#include <netinet/in.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nfnetlink.h>

#include <linux/types.h>
#include <linux/netfilter/nfnetlink_queue.h>

#include <libnetfilter_queue/libnetfilter_queue.h>

#include "AttackManager.hpp"
#include "VictimManager.hpp"
#include "../Modules/FilterModule.hpp"
#include "../Modules/PolicyModule.hpp"

class PacketManager {
	public:
		PacketManager(int input_queue_num, int output_queue_num, PolicyModule * policy, FilterModule * filter);
		~PacketManager();

	private:
		struct nfq_handle * my_netfilterqueue_handle;
		AttackManager * attack_manager;
		VictimManager * victim_manager;
};

#endif /* PACKETMANAGER_HPP */
