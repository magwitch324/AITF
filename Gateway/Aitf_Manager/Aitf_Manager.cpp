#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "../logger.hpp"
#include "Aitf_Manager.hpp"
#include "Udp_Server.hpp"

#define DST_INDEX 77

Aitf_Manager::Aitf_Manager(){

}

void Aitf_Manager::run(){
	server = new Udp_Server(this);
	server->start();
}

void Aitf_Manager::start_thread(){
	log(logINFO) << "starting Aitf_Manager";
	aitf_thread = boost::thread(&Aitf_Manager::run, this);

}

void Aitf_Manager::stop_thread(){
	log(logINFO) << "Stopping aitf manager";
	server->stop();
	delete(server);
	log(logDEBUG2) << "server deltetd";
	aitf_thread.interrupt();
	aitf_thread.join();
	log(logINFO) << "Aitf manager stopped";
}


void Aitf_Manager::packet_arrived(std::vector<uint8_t> recv_buf){
	log(logDEBUG2) << "Checking message type";

	//extract the type of message
	uint8_t msg_type = recv_buf[0];
	log(logDEBUG) << "Message type: " << (int) msg_type; 

	switch(msg_type){
		case 0: 
			handle_request(recv_buf);
			break;
		case 1:
			break;
		case 2:
			break;
		default:
			log(logWARNING) << "Invalid message type";
			break;
	}
}

void Aitf_Manager::handle_request(std::vector<uint8_t> message){
	log(logDEBUG2) << "Received filter request";

	//check the message length
	if(message.size() == 84){

		//pull out the dst_ip
		uint32_t dst_ip;
		memcpy(&dst_ip, &message[DST_INDEX], 4);

		//if the victim is within its rate limit
		if(aitf_hosts_table->check_from_rate(dst_ip)){

			//apply temp filter
			//check shadow table

			//check route length

			//if 1 handle attacker

			//if more

			//insert gateway filter for response

			//send handshake
		}
	}
	else{
		log(logWARNING) << "Message size is incorrect!!: " << message.size();
	}
}
