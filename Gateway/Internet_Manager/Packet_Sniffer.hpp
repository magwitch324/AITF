#ifndef PACKET_SNIFFER_HPP
#define PACKET_SNIFFER_HPP
#include <stdint.h>
#include <vector>
#include "../Flow.hpp"

class Packet_Sniffer{
	public:
		virtual bool is_allowed(Flow flow, std::vector<uint8_t> payload) = 0;
};

#endif
