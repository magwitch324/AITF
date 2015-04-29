/*
 * VictimManager.hpp
 *
 *  Created on: Apr 24, 2015
 *      Author: Sean
 */

#ifndef VICTIMMANAGER_HPP_
#define VICTIMMANAGER_HPP_

#include "HostManager.hpp"

class VictimManager : public HostManager{
	public:
		VictimManager(struct nfq_handle * a_nfq_handle, int victim_queue_num, PolicyModule * pol);
		~VictimManager();
	protected:
		int packetCallbackFunc(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data);
		PolicyModule * policy;
};


#endif /* VICTIMMANAGER_HPP_ */
