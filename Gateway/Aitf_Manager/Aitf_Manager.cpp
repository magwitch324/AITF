#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "../logger.hpp"
#include "Aitf_Manager.hpp"
#include "Udp_Server.hpp"
#include "../Hasher.hpp"

#define DST_REQ_INDEX 78

Aitf_Manager::Aitf_Manager(){

}

void Aitf_Manager::run(){
	server = new Udp_Server(this);
	server->start();
}

void Aitf_Manager::timeout_run(){
	timeout_work.reset(new boost::asio::io_service::work(timeout_io));
	timeout_io.run();
}

void Aitf_Manager::start_thread(){
	log(logINFO) << "starting Aitf_Manager";
	aitf_thread = boost::thread(&Aitf_Manager::run, this);
	timeout_thread = boost::thread(&Aitf_Manager::timeout_run, this);

}

void Aitf_Manager::stop_thread(){
	log(logINFO) << "Stopping aitf manager";


	server->stop();

	//stop timeout thread
	log(logDEBUG2) << "stopping timeout thread";
	timeout_work.reset();
	timeout_io.stop();
	timeout_thread.interrupt();
	timeout_thread.join();
	log(logDEBUG2) << "timeout thread stopped";

	log(logDEBUG2) << "stopping aitf thread";
	aitf_thread.interrupt();
	aitf_thread.join();
	log(logDEBUG2) << "aitf thread stopped";



	log(logDEBUG2) << "deleting server";
	delete(server);
	log(logDEBUG2) << "server deltetd";


	log(logINFO) << "Aitf manager stopped";
}


void Aitf_Manager::packet_arrived(std::vector<uint8_t> recv_buf){
	log(logDEBUG2) << "Checking message type";

	//extract the type of message
	uint8_t msg_type = recv_buf[0];
	log(logDEBUG) << "Message type: " << (int) msg_type; 

	switch(msg_type){
		case 0: //Filter request
			handle_filter_request(recv_buf);
			break;
		case 1: //Handshake request
			handle_handshake_request(recv_buf);
			break;
		case 2: //Filter request reply
			break;
		default:
			log(logWARNING) << "Invalid message type";
			break;
	}
}

void Aitf_Manager::handle_handshake_request(std::vector<uint8_t> message){
	log(logDEBUG2) << "Received handshake request";

	//check the message length
	if(message.size() == 94){


		//pullout the requester gateway IP
		uint32_t gtw_ip;
		memcpy(&gtw_ip, &message[1], 4);

		//if the gateway is within its rate limit
		if(aitf_hosts_table->check_from_rate(gtw_ip)){

			std::vector<uint8_t> flow(&message[5], &message[5]+81);

			//pull out the dst_ip, atk_gtw_value and random value
			uint32_t dst_ip;
			uint64_t r_value;
			uint32_t atk_gtw_ip;
			uint8_t ptr = flow[4];
			if(ptr > 5){
				log(logERROR) << "Flow pointer too big!!!";
				return;
			}
			memcpy(&dst_ip, &flow[77], 4);
			memcpy(&atk_gtw_ip, &flow[5+ptr*12], 4);
			memcpy(&r_value, &flow[5+ptr*12+4], 8);
			log(logDEBUG) << "dst: " << dst_ip << " atk_gtw_ip: " << atk_gtw_ip << " r_value: " << r_value;

			//check that the random value in the flow is correct
			//compute the hash
			uint64_t actual = Hasher::hash(*gateway_key, (unsigned char*) &dst_ip, 4);
			log(logDEBUG2) << "actual: " << actual << " r_value: " << r_value;
			if(r_value == actual){
				log(logDEBUG) << "CORRECT HASH";
			}
			else{
				log(logDEBUG) << "INCORRECT HASH";
			}
		}
	}
}

void Aitf_Manager::handle_filter_request(std::vector<uint8_t> message){
	log(logDEBUG2) << "Received filter request";

	//check the message length
	if(message.size() == 83){

		//pull out the dst_ip
		uint32_t dst_ip;
		memcpy(&dst_ip, &message[DST_REQ_INDEX], 4);

		//if the victim is within its rate limit
		if(aitf_hosts_table->check_from_rate(dst_ip)){

			std::vector<uint8_t> flow(&message[1], &message[1]+81);
			for(int i = 0; i < 83; i++){
				log(logERROR) << (int) message[i];
			}
			//apply temp filter
			filter_table->add_temp_filter(flow);

			log(logDEBUG2) << "checking shadow table";
			//check shadow table
			bool repeat_request = shadow_table->is_flow_filtered(flow);

			//check route length
			uint8_t flow_ptr = flow[4];

			//if there is only one gateway in the list, then this gateway
			//must deal with it
			if(flow_ptr == 0){
				log(logDEBUG2) << "dealing with attacker";
				uint32_t src_ip;
				memcpy(&src_ip, &flow[0], 4);
				//if the host is AITF enabled
				if(aitf_hosts_table->contains_host(src_ip)){
					//if this is a repeat offense
					if(repeat_request){
						filter_table->add_long_filter(flow);
					}
					else{
						//contact host
						//TODO CONTACT HOST!!!
						//send udp packet
						boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(timeout_io, boost::posix_time::seconds(TEMP_TIME)));
						timer->async_wait(boost::bind(&Aitf_Manager::unresponsive_host, this, boost::asio::placeholders::error, timer, flow));
					}
				}
			}
			else{
				//insert gateway filter for response

				//send handshake
			}
		}
	}
	else{
		log(logWARNING) << "Message size is incorrect!!: " << message.size();
	}
}

void Aitf_Manager::unresponsive_host(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, std::vector<uint8_t> flow){
	log(logDEBUG2) << "In unresponsive_host callback";

	//if the flow has not been added to the table then cut off the client
	if(!shadow_table->is_flow_filtered(flow)){
		filter_table->add_long_filter(flow);
	}

}
