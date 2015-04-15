#ifndef PACKET_LISTENER
#define PACKET_LISTENER
#include <stdint.h>
#include <vector>

class Packet_Listener{
public:
	virtual void packet_arrived(uint8_t msg_type, std::vector<uint8_t> recv_buf) = 0;
};

#endif