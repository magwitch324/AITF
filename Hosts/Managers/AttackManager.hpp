/*
 * AttackManager.hpp
 */

#ifndef ATTACKMANAGER_HPP
#define ATTACKMANAGER_HPP

#include "HostManager.hpp"
#include "../Modules/FilterModule.hpp"

class AttackManager : public HostManager{
	public:
		AttackManager(uint32_t ip, struct nfq_handle * a_nfq_handle, int attack_queue_num, FilterModule * fil);
		~AttackManager();
		int packetCallbackFunc(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data);
		void addFilter(uint32_t dest, int timeout);

	protected:
		void sendFilterResponse(uint32_t dest);
		uint32_t my_ip;
		FilterModule * filter;
};

#endif /* ATTACKMANAGER_HPP_ */
