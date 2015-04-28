#include <vector>
#include <stdlib.h>
#include <boost/asio.hpp>
#include <boost/thread.hpp>


using namespace boost::asio;
using boost::asio::ip::udp;



std::vector<uint8_t> filter_request(){
	uint8_t msg_type = 0;
	uint32_t src_ip = 99;
	uint8_t ptr = 1;

	uint32_t gtw1_ip = 20;
	uint64_t gtw1_rval = 120;

	uint32_t gtw2_ip = 21;
	uint64_t gtw2_rval = 127;

	uint32_t gtw3_ip = 22;
	uint64_t gtw3_rval = 122;

	uint32_t gtw4_ip = 23;
	uint64_t gtw4_rval = 123;

	uint32_t gtw5_ip = 24;
	uint64_t gtw5_rval = 124;

	uint32_t gtw6_ip = 25;
	uint64_t gtw6_rval = 125;

	uint32_t dst_ip = 99;

	uint8_t escaltion = 1;


	uint8_t message [83] = {};

	//copy in the info
	memcpy(&message[0], &msg_type, 1);

	memcpy(&message[1], &src_ip, 4);

	memcpy(&message[5], &ptr, 1);

	memcpy(&message[6], &gtw1_ip, 4);
	memcpy(&message[10], &gtw1_rval, 8);

	memcpy(&message[18], &gtw2_ip, 4);
	memcpy(&message[22], &gtw2_rval, 8);

	memcpy(&message[30], &gtw3_ip, 4);
	memcpy(&message[34], &gtw3_rval, 8);

	memcpy(&message[42], &gtw4_ip, 4);
	memcpy(&message[46], &gtw4_rval, 8);

	memcpy(&message[54], &gtw5_ip, 4);
	memcpy(&message[58], &gtw5_rval, 8);

	memcpy(&message[66], &gtw6_ip, 4);
	memcpy(&message[70], &gtw6_rval, 8);


	memcpy(&message[78], &dst_ip, 4);

	memcpy(&message[82], &escaltion, 1);

	std::vector<uint8_t> test(&message[0], &message[0]+83);
	//std::vector<uint8_t> test(83);
	return test;
}

std::vector<uint8_t> handshake_request(){
	uint8_t message[94] = {};
	uint8_t type = 1;

	uint32_t src_gtw = 2;
	uint64_t r_val1 = UINT64_C(14140794901464777107);


	uint32_t dst_ip = 99;

	message[0] = type;
	memcpy(&message[1], &src_gtw, 4);
	memcpy(&message[14], &r_val1, 8);

	memcpy(&message[82], &dst_ip, 4);

	std::vector<uint8_t> test(&message[0], &message[0]+94);
	return test;
}


int main(int argc, char* argv[]){
	boost::asio::io_service io_service;
	udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(),"127.0.0.1","50000");
	udp::endpoint receiver_endpoint = *resolver.resolve(query);

	udp::socket socket(io_service);
	socket.open(udp::v4());

	std::vector<uint8_t> message(0);

	if(argc == 2){
		int type = atoi(argv[1]);
		switch(type){
			case 1:
				message = handshake_request();
		}
	}
	else{
		message = filter_request();
	}


	socket.send_to(boost::asio::buffer(message), receiver_endpoint);
}


