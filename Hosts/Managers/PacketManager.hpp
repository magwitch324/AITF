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
		PacketManager(uint32_t ip, int input_queue_num, int output_queue_num, PolicyModule * policy, FilterModule * filter);
		~PacketManager();
		void aitfCommunication(std::vector<uint8_t> recv_buf);

	private:
		struct nfq_handle * my_netfilterqueue_handle;
		AttackManager * attack_manager;
		VictimManager * victim_manager;
		uint32_t ip;
};

#endif /* PACKETMANAGER_HPP */
