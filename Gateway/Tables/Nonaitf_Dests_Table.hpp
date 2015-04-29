#ifndef NONAITF_DESTS_TABLE_HPP
#define NONAITF_DESTS_TABLE_HPP
#include <unordered_set>

class Nonaitf_Dests_Table{
	public:
		bool is_nonaitf(uint32_t dst_ip);
	private:
		std::unordered_set<uint32_t> non_aitf_dests;
};

extern Nonaitf_Dests_Table* nonaitf_dests_table;

#endif