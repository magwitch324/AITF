/*
 * FilterModule.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: Sean
 */

#include "FilterModule.hpp"
#include "../logger.hpp"
#include "../Constants.h"
#include "../Helpers.hpp"
#include "../Flow.hpp"

FilterModule::FilterModule() {
	llog(logINFO) << "Creating filter module";
	activeFilters = new Async_Auto_Flow_Table( "../activeFilters.log", 1000);
	activeFilters->start_thread();
	//activeFilters = new Async_Auto_Table();

}

FilterModule::~FilterModule() {
	llog(logINFO) << "Deleting filter module";
	activeFilters->stop_thread();
	delete activeFilters;
}

bool FilterModule::shouldFilter(uint32_t source_ip, uint32_t destination_ip) {
	Flow flow;
	flow.src_ip = source_ip;
	flow.dst_ip = destination_ip;

	return (activeFilters->getValue(flow) > 0);
}

void FilterModule::addNewFilter(uint32_t source_ip, uint32_t destination_ip, int timeout) {

#ifdef LYING_ATTACKER
	timeout = (std::rand() % ( timeout - T_TEMP_MS*2) ) + T_TEMP_MS*2;
#endif

	llog(logINFO) << "Setting new filter for all traffic from " << Helpers::ip_to_string(source_ip) << " to " << Helpers::ip_to_string(destination_ip) << " for " << Helpers::ip_to_string(destination_ip) << "ms";

	Flow flow;
	flow.src_ip = source_ip;
	flow.dst_ip = destination_ip;

	activeFilters->addValue(flow, 1, -1, timeout);
}
