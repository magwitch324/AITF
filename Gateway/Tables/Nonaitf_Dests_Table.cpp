#include "Nonaitf_Dests_Table.hpp"
#include "../logger.hpp"

void Nonaitf_Dests_Table::add_dst(uint32_t dst_ip){
	non_aitf_dests.emplace(dst_ip);
}

bool Nonaitf_Dests_Table::is_nonaitf(uint32_t dst_ip){
	log(logDEBUG2) << "Checking AITF dests table";
	int num = non_aitf_dests.count(dst_ip);
	log(logDEBUG) << "Done checking table";
	return num == 1;
}