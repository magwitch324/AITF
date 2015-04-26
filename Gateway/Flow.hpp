#ifndef FLOW_HPP
#define FLOW_HPP

#include <vector>
#include <stdint.h>
#include <boost/functional/hash.hpp>
#include "logger.hpp"

class Flow {
	public:
		Flow(std::vector<uint8_t> new_flow);
		Flow(const Flow& old_flow);
		bool operator==(const Flow &other) const;
		std::vector<uint8_t> to_byte_vector();
		uint32_t get_gtw_ip_at(uint8_t pointer);
		uint64_t get_gtw_rvalue_at(uint8_t pointer);
		void set_gtw_ip_at(uint8_t pointer, uint32_t new_value);
		void set_gtw_rvalue_at(uint8_t pointer, uint64_t new_value);

		uint32_t src_ip = 0;
		uint8_t pointer = 0;

		uint32_t gtw0_ip = 0;
		uint64_t gtw0_rvalue = 0;

		uint32_t gtw1_ip = 0;
		uint64_t gtw1_rvalue = 0;

		uint32_t gtw2_ip = 0;
		uint64_t gtw2_rvalue = 0;

		uint32_t gtw3_ip = 0;
		uint64_t gtw3_rvalue = 0;

		uint32_t gtw4_ip = 0;
		uint64_t gtw4_rvalue = 0;

		uint32_t gtw5_ip = 0;
		uint64_t gtw5_rvalue = 0;

		uint32_t dst_ip = 0;
};

struct KeyHasher{
	std::size_t operator() (const Flow& flow) const{
		std::size_t seed = 0;
        boost::hash_combine(seed, flow.src_ip);
        boost::hash_combine(seed, flow.pointer);

        boost::hash_combine(seed, flow.gtw0_ip);
        boost::hash_combine(seed, flow.gtw0_rvalue);

        boost::hash_combine(seed, flow.gtw1_ip);
        boost::hash_combine(seed, flow.gtw1_rvalue);

        boost::hash_combine(seed, flow.gtw2_ip);
        boost::hash_combine(seed, flow.gtw2_rvalue);

        boost::hash_combine(seed, flow.gtw3_ip);
        boost::hash_combine(seed, flow.gtw3_rvalue);

        boost::hash_combine(seed, flow.gtw4_ip);
        boost::hash_combine(seed, flow.gtw4_rvalue);

        boost::hash_combine(seed, flow.gtw5_ip);
        boost::hash_combine(seed, flow.gtw5_rvalue);

        return seed;
	}
};

#endif