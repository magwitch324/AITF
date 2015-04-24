/*
 * AttackManager.hpp
 */

#ifndef ATTACKMANAGER_HPP
#define ATTACKMANAGER_HPP

#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

class AttackManager {
	public:
		void aitfCommunication();
		unsigned int performFilter();

};

#endif /* ATTACKMANAGER_HPP_ */
