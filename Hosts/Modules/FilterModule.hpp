/*
 * FilterModule.hpp
 *
 *  Created on: Apr 29, 2015
 *      Author: Sean
 */

#ifndef FILTERMODULE_HPP
#define FILTERMODULE_HPP

#include "../Tables/Async_Auto_Flow_Table.hpp"

class FilterModule {
	public:
		FilterModule();
		~FilterModule();
		bool shouldFilter(uint32_t source_ip, uint32_t destination_ip);
		void addNewFilter(uint32_t source_ip, uint32_t destination_ip, int timeout);

	private:
		Async_Auto_Flow_Table * activeFilters;
};

#endif /* FILTERMODULE_HPP_ */
