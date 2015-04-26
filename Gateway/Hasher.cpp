#include <openssl/hmac.h>
#include <cstring>
#include "Hasher.hpp"

uint64_t Hasher::hash(std::string key, unsigned char* data, int data_len){
	unsigned char* result;

	result = HMAC(EVP_sha1(), key.c_str(), key.length(), data, data_len, NULL, NULL);

	uint64_t hash;
	memcpy(&hash, result, 8);

	return hash;
}