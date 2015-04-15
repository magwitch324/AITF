#ifndef PACKET_LISTENER
#define PACKET_LISTENER
#include <stdint.h>
#include <vector>

class Packet_Listener{
public:
	virtual void packet_arrived(std::vector<uint8_t> recv_buf) = 0;
};

#endif