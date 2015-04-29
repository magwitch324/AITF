/*
 * FilterModule.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: Sean
 */

#include "FilterModule.hpp"

FilterModule::FilterModule() {

	activeFilters = new Async_Auto_Table( "../activeFilters.log", 1000);
	//activeFilters = new Async_Auto_Table();

}

FilterModule::~FilterModule() {
	delete activeFilters;
}

int FilterModule::sentPacket(uint32_t destination_ip) {
	if(activeFilters->getValue(destination_ip) > 0 ) {
		return -1;
	}
	return 1;
}

void FilterModule::addNewFilter(uint32_t destination_ip, int timeout) {
	activeFilters->addValue(destination_ip, 1, -1, timeout);
}
