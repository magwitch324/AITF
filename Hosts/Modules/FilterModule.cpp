/*
 * FilterModule.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: Sean
 */

#include "FilterModule.hpp"
#include "../logger.hpp"

FilterModule::FilterModule() {
	llog(logDEBUG) << "Creating filter module";
	activeFilters = new Async_Auto_Table( "../activeFilters.log", 1000);
	//activeFilters = new Async_Auto_Table();

}

FilterModule::~FilterModule() {
	llog(logDEBUG) << "Deleting filter module";
	delete activeFilters;
}

int FilterModule::sentPacket(uint32_t destination_ip) {
	
	if(activeFilters->getValue(destination_ip) > 0 ) {
		llog(logDEBUG) << "You should Filter " << destination_ip;
		return -1;
	}
	llog(logDEBUG) << "You should pass " << destination_ip;
	return 1;
}

void FilterModule::addNewFilter(uint32_t destination_ip, int timeout) {
	llog(logDEBUG) << "Setting new filter to " << destination_ip;
	activeFilters->addValue(destination_ip, 1, -1, timeout);
}
