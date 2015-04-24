/*
 * VictimManager.hpp
 *
 *  Created on: Apr 24, 2015
 *      Author: Sean
 */

#ifndef VICTIMMANAGER_HPP_
#define VICTIMMANAGER_HPP_

#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

class VictimManager {
	public:
		void performStatistics();

};


#endif /* VICTIMMANAGER_HPP_ */
