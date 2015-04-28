#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "../logger.hpp"
#include "Internet_Manager.hpp"
#include "../Hasher.hpp"

void Internet_Manager::packet_callback(std::vector<uint8_t> packet){
	//Determine if the packet has an RR header

	//if it does have an RR header
	//add IP and RV to header

	//If it does not have an RR header
	//consult non-AITF endhost table
	//If dest is present, approve packet.
	//Else create RR header and add ip and rv

	//Check if a filter exists for the packet
	Flow flow;
	if(filter_table->flow_is_filtered(flow)){

	}
	//If no, allow the packet

	//If yes
	//Is the src the same as the first gateway IP
	//If yes handle handshake
	//If no drop the packet

}

void Internet_Manager::handle_handshake(std::vector<uint8_t> message){
	//check that the message is the right size
	if(message.size() == 91){
		//check that the msg type is correct
		if(message[0] == 2){
			//grab the nonce and flow
			Flow flow(std::vector<uint8_t>(&message[2], &message[2]+81));
			uint64_t nonce;
			memcpy(&nonce, &message[83], 8);

			//compute the expected nonce
			uint64_t actual = Hasher::hash(*gateway_key, &flow.to_byte_vector()[0], 81);

			//grab the atk_gtw ip address
			uint32_t gtw_ip = flow.get_gtw_ip_at(message[1]);

			//if the nonce is the one that this gateway sent
			if(nonce == actual){
				//this means that the rvalue for the gateway was wrong
				//add a filter so that only the correct value is accepted

				//grab the correct rvalue
				uint64_t rvalue = flow.get_gtw_rvalue_at(message[1]);

				filter_table->add_gtw_rvalue(flow.dst_ip, gtw_ip, rvalue);

			}
			else{
				//the rvalue is correct, just reflect the message back
				//TODO: send message to gtw_ip
			}

		}
	}

}