#include <unordered_map>
#include "Filter_Info.hpp"

class Aitf_Hosts_Table{
	
	public:
		Aitf_Hosts_Table();
		bool check_from_rate(uint32_t ip);


	private:
		std::unordered_map<uint32_t, Filter_Info> hosts;
};