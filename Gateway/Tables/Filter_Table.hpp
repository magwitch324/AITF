#ifndef FILTER_TABLE_HPP
#define FILTER_TABLE_HPP

#include <unordered_map>
#include "Filter_Set.hpp"
#include "Timed_Table.hpp"

class Filter_Table : public Timed_Table{

	public:
		void add_temp_filter(Flow flow);
		void add_long_filter(Flow flow);
		void add_gtw_rvalue(uint32_t dst_ip, uint32_t gtw_ip, uint64_t rvalue);
		int attempt_count(Flow flow);
		bool flow_is_filtered(Flow flow);

	private:
		std::unordered_map<uint32_t, Filter_Set> filters;

};

extern Filter_Table* filter_table;
extern Filter_Table* shadow_table;

#endif
