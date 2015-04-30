/*
 * VictimManager.hpp
 *
 *  Created on: Apr 24, 2015
 *      Author: Sean
 */

#ifndef VICTIMMANAGER_HPP_
#define VICTIMMANAGER_HPP_

#include "HostManager.hpp"
#include "../Modules/PolicyModule.hpp"

class VictimManager : public HostManager{
	public:
		VictimManager(uint32_t ip, uint32_t gtw, struct nfq_handle * a_nfq_handle, int victim_queue_num, PolicyModule * pol);
		~VictimManager();
		int packetCallbackFunc(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data);

	protected:
		PolicyModule * policy;
		uint32_t my_ip;
		uint32_t my_gtw;
		void SendFilterRequest(Flow * flow);
};


#endif /* VICTIMMANAGER_HPP_ */
