#ifndef FILTER_TABLE_HPP
#define FILTER_TABLE_HPP

#include <unordered_map>
#include "Filter_Set.hpp"
#include "Timed_Table.hpp"

class Filter_Table : public Timed_Table{

	public:
		void add_temp_filter(uint8_t flow[]);

	private:
		std::unordered_map<uint32_t, Filter_Set*> filters;

};

extern Filter_Table* filter_table;

#endif
