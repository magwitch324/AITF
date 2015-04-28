#ifndef INTERNET_MANAGER_HPP
#define INTERNET_MANAGER_HPP
#include "../Tables/Filter_Table.hpp"

class Internet_Manager{
public:
	void packet_callback(std::vector<uint8_t> packet);
	void handle_handshake(std::vector<uint8_t> message);

};

#endif