/*
 * AttackManager.hpp
 */

#ifndef ATTACKMANAGER_HPP
#define ATTACKMANAGER_HPP

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

class AttackManager {
	public:
		static void aitfCommunication();
		static int performFilter();

};

#endif /* ATTACKMANAGER_HPP_ */
