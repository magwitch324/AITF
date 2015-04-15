#ifndef PACKET_LISTENER_HPP
#define PACKET_LISTENER_HPP
#include <stdint.h>
#include <vector>

class Packet_Listener{
	public:
		virtual void packet_arrived(std::vector<uint8_t> recv_buf) = 0;
};

#endif
