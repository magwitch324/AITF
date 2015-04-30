/*
 * PolicyModule.hpp
 *
 *  Created on: Apr 29, 2015
 *      Author: Sean
 */

#ifndef POLICYMODULE_HPP
#define POLICYMODULE_HPP

#include "../Tables/Async_Auto_Table.hpp"
#include "../Tables/Async_Auto_Flow_Table.hpp"
#include "../Flow.hpp"

class PolicyModule {
	public:
		PolicyModule();
		~PolicyModule();
		int receivedPacket(uint32_t source_ip, int size);
		int receivedPacket(Flow flow, int size);
	private:
		std::unordered_map<uint32_t, int> defaults;
		Async_Auto_Flow_Table * bandwidthUsage;
		Async_Auto_Flow_Table * filterRequests;
};

#endif /* POLICYMODULE_HPP_ */
