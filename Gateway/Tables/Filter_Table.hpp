#ifndef FILTER_TABLE_HPP
#define FILTER_TABLE_HPP

#include <unordered_map>
#include "Filter_Set.hpp"
#include "Timed_Table.hpp"

class Filter_Table : public Timed_Table{

	public:
		~Filter_Table();
		void add_temp_filter(uint8_t flow[]);
		void add_long_filter(uint8_t flow[]);
		bool is_flow_filtered(uint8_t flow[]);

	private:
		std::unordered_map<uint32_t, Filter_Set*> filters;

};

extern Filter_Table* filter_table;
extern Filter_Table* shadow_table;

#endif
