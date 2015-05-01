/*
 * FilterModule.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: Sean
 */

#include "FilterModule.hpp"
#include "../logger.hpp"
#include "../Constants.h"

FilterModule::FilterModule() {
	llog(logDEBUG) << "Creating filter module";
	activeFilters = new Async_Auto_Table( "../activeFilters.log", 1000);
	activeFilters->start_thread();
	//activeFilters = new Async_Auto_Table();

}

FilterModule::~FilterModule() {
	llog(logDEBUG) << "Deleting filter module";
	activeFilters->stop_thread();
	delete activeFilters;
}

bool FilterModule::shouldFilter(uint32_t destination_ip) {
	return activeFilters->getValue(destination_ip) > 0;
}

void FilterModule::addNewFilter(uint32_t destination_ip, int timeout) {
	llog(logDEBUG) << "Setting new filter to " << destination_ip;

#ifdef LYING_ATTACKER
	timeout = (std::rand() % ( timeout - T_TEMP_MS*2) ) + T_TEMP_MS*2;
#endif

	activeFilters->addValue(destination_ip, 1, -1, timeout);
}
