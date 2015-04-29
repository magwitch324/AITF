/*
 * HostManager.hpp
 *
 *  Created on: Apr 28, 2015
 *      Author: Sean
 */

#ifndef HOSTMANAGER_HPP
#define HOSTMANAGER_HPP

#include <libmnl/libmnl.h>
#include <netinet/in.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nfnetlink.h>

#include <linux/types.h>
#include <linux/netfilter/nfnetlink_queue.h>

#include <libnetfilter_queue/libnetfilter_queue.h>

#include <boost/atomic.hpp>
#include <boost/thread.hpp>

class HostManager {
	public:
		HostManager(struct nfq_handle * a_nfq_handle, int queue_num);
		virtual ~HostManager();

	protected:
		enum {STARTING, STARTED, ENDING, ENDED};
		mutable boost::atomic<int> state;
		boost::thread * packet_thread;
		nfq_q_handle * queue_handle;
		void packetRecieveThreadFunc(struct nfq_handle * a_nfq_handle);
		static u_int32_t getPktID (struct nfq_data *tb);
		virtual int packetCallbackFunc(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data) = 0;

};

static int callbackForward(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data){
	HostManager * temp = dynamic_cast<HostManager *> (data);
	return temp->packetCallbackFunc(qh, nfmsg, nfad, NULL);
}


#endif /* HOSTMANAGER_HPP_ */
