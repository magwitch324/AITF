/*
 * PolicyModule.hpp
 *
 *  Created on: Apr 29, 2015
 *      Author: Sean
 */

#ifndef POLICYMODULE_HPP
#define POLICYMODULE_HPP

#include "../Tables/Async_Auto_Table.hpp"
#include "../Flow.hpp"

class PolicyModule {
	public:
		PolicyModule();
		~PolicyModule();
		int receivedPacket(uint32_t source_ip, int size);
		int receivedPacket(Flow flow, int size);
	private:
		std::unordered_map<uint32_t, int> defaults;
		uint32_t strToIP(const char * str_ip);
		Async_Auto_Table * bandwidthUsage;
		Async_Auto_Table * filterRequests;
		std::unordered_map<Flow, int, KeyHasher> flow_filters;
};

#endif /* POLICYMODULE_HPP_ */
