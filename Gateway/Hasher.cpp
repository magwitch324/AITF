#include <openssl/hmac.h>
#include "Hasher.hpp"

std::vector<uint8_t> Hasher::hash(std::string key, unsigned char* data, int data_len){
	unsigned char* result;

	result = HMAC(EVP_sha1(), key.c_str(), key.length(), data, data_len, NULL, NULL);

	std::vector<uint8_t> hash(result, result+8);

	return hash;
}