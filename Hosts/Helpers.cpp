#include <netinet/ip.h>
#include <arpa/inet.h>
#include "Helpers.hpp"

std::string Helpers::ip_to_string(uint32_t ip){
	uint32_t ipINT = ip;
	in_addr* addr = (in_addr*)(&ipINT);
	return std::string(inet_ntoa(*addr));
}