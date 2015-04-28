#ifndef PACKET_SNIFFER_HPP
#define PACKET_SNIFFER_HPP
#include <stdint.h>
#include <vector>

class Packet_Sniffer{
	public:
		virtual void packet_callback(std::vector<uint8_t> recv_buf) = 0;
};

#endif
