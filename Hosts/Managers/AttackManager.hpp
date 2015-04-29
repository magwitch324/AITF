/*
 * AttackManager.hpp
 */

#ifndef ATTACKMANAGER_HPP
#define ATTACKMANAGER_HPP

#include "HostManager.hpp"
#include "Modules/FilterModule.hpp"

class AttackManager : public HostManager{
	public:
		AttackManager(struct nfq_handle * a_nfq_handle, int attack_queue_num, FilterModule * fil);
		~AttackManager();
	protected:
		int packetCallbackFunc(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data);
		FilterModule * filter;
};

#endif /* ATTACKMANAGER_HPP_ */
