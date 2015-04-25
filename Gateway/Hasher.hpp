#ifndef HASHER_HPP
#define HASHER_HPP
#include <vector>
#include <string>

class Hasher{
	public:
		static std::vector<uint8_t> hash(std::string key, unsigned char* data, int data_len);
};

extern std::string* gateway_key;

#endif
