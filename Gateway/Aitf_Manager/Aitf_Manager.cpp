#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/asio.hpp>
#include "../logger.hpp"
#include "Aitf_Manager.hpp"
#include "Udp_Server.hpp"
#include "../Hasher.hpp"
#include "../Constants.hpp"
#include "../Helpers.hpp"

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
		case 2: //Handshake Part 2
			//We should never receive one of these
			break;
		case 3: //Handshake part 3
			handle_handshake_finish(recv_buf);
			break;
		case 4: //Attack filter request
			//We should never receive one of these
			break;
		case 5: //Filter request reply
			handle_filter_reply(recv_buf);
			break;
		default:
			log(logWARNING) << "Invalid message type";
			break;
	}
}

void Aitf_Manager::handle_filter_reply(std::vector<uint8_t> message){
	if(message.size() == 9){
		log(logINFO) << "Received filter reply";
		Flow flow;
		memcpy(&flow.src_ip, &message[1], 4);
		memcpy(&flow.dst_ip, &message[5], 4);
		flow.gtw0_ip = MY_IP;
		flow.gtw0_rvalue = Hasher::hash(*gateway_key, (unsigned char*) &flow.dst_ip, 4);

		log(logDEBUG2) << Helpers::ip_to_string(flow.src_ip);

		log(logDEBUG2) << Helpers::ip_to_string(flow.gtw0_ip);

		log(logDEBUG2) << flow.gtw0_rvalue;

		log(logDEBUG2) << Helpers::ip_to_string(flow.dst_ip);

		//if there was a filter request sent for this flow
		if(filter_table->attempt_count(flow) > 0){
			log(logDEBUG2) << "Successfully replied to request to stop";
			//add to the shadow table
			shadow_table->add_long_filter(flow);
		}
		else{
			log(logDEBUG2) << "UNKNOWN FLOW";
		}
	}
}

void Aitf_Manager::handle_handshake_finish(std::vector<uint8_t> message){
	log(logINFO) << "Received handshake finish";

	if(message.size() == 91){
		Flow flow(std::vector<uint8_t>(&message[2], &message[2] + 81));
		//check that the nonce is correct
		uint64_t nonce;
		memcpy(&nonce, &message[83], 8);
		uint64_t actual = Hasher::hash(*gateway_key, &flow.to_byte_vector()[0], 81);

		//if the nonce is correct then deal with the attacker
		if(nonce == actual){
			//remove downstream gateways before adding filter
			for(int i = message[1] + 1; i < 6; i++){
				flow.set_gtw_ip_at(i, 0);
				flow.set_gtw_rvalue_at(i, 0);
			}
			flow.pointer = message[1];

			log(logDEBUG2) << "attack filter src: " << Helpers::ip_to_string(flow.src_ip);
			log(logDEBUG2) << "attack filter dst: " << Helpers::ip_to_string(flow.dst_ip);
			for(int i = 0; i <= flow.pointer; i++){
				log(logDEBUG2) << "attack gtw ip " << i << " "<< Helpers::ip_to_string(flow.get_gtw_ip_at(i));
				log(logDEBUG2) << "attack gtw rvalue " << i << " "<< flow.get_gtw_rvalue_at(i);
			}

			filter_table->add_temp_filter(flow);

#ifndef LYING
			log(logDEBUG2) << "checking shadow table";
			//check shadow table
			int request_attempts = shadow_table->attempt_count(flow);

			deal_with_attacker(flow, request_attempts);
#endif
		}
	}
}

void Aitf_Manager::handle_handshake_request(std::vector<uint8_t> message){
	log(logINFO) << "Received handshake request";

	//check the message length
	if(message.size() == 95){


		//pullout the requester gateway IP
		uint32_t gtw_ip;
		memcpy(&gtw_ip, &message[1], 4);

		//if the gateway is within its rate limit
		if(aitf_hosts_table->check_from_rate(gtw_ip)){

			//create the flow
			Flow flow(std::vector<uint8_t>(&message[6], &message[6]+81));

			uint8_t this_gtw_ptr = message[5];
			log(logDEBUG2) << "THE GATEWAY POINTER IS: " << (int) this_gtw_ptr;

			uint64_t r_value = flow.get_gtw_rvalue_at(this_gtw_ptr);
			uint32_t atk_gtw_ip = flow.get_gtw_ip_at(this_gtw_ptr);

			log(logDEBUG) << "dst: " << Helpers::ip_to_string(flow.dst_ip) << " atk_gtw_ip: " << Helpers::ip_to_string(atk_gtw_ip) << " r_value: " << r_value;

			//check that the random value in the flow is correct
			//compute the hash
			uint64_t actual = Hasher::hash(*gateway_key, (unsigned char*) &flow.dst_ip, 4);
			log(logDEBUG2) << "actual: " << actual << " r_value: " << r_value;


			//construct the reply
			std::vector<uint8_t> reply(91);
			reply[0] = 2;
			reply[1] = message[5];
			if(r_value == actual){
				//construct the reply with a new nonce
				log(logDEBUG) << "CORRECT HASH";
				uint64_t nonce = Hasher::hash(*gateway_key, (unsigned char*) &flow.to_byte_vector()[0], 81);
				memcpy(&reply[2], &flow.to_byte_vector()[0], 81);
				memcpy(&reply[83], &nonce, 8);
			}
			else{
				//send back the reply with a correct r_value and same nonce
				log(logDEBUG) << "INCORRECT HASH";
				Flow modified_flow(flow);
				//modified_flow.set_gtw_rvalue_at(this_gtw_ptr, actual);
				reply.resize(99);
				memcpy(&reply[91], &actual, 8);

				log(logDEBUG2) << "src: " << Helpers::ip_to_string(modified_flow.src_ip);
				log(logDEBUG2) << "dst: " << Helpers::ip_to_string(modified_flow.dst_ip);
				for(int i = 0; i <= modified_flow.pointer; i++){
					log(logDEBUG2) << "gtw ip " << i << " "<< Helpers::ip_to_string(modified_flow.get_gtw_ip_at(i));
					log(logDEBUG2) << "gtw rvalue " << i << " "<< modified_flow.get_gtw_rvalue_at(i);
				}

				//copy in the flow to to reply
				memcpy(&reply[2], &modified_flow.to_byte_vector()[0], 81);
				//copy in the nonce from the original message
				memcpy(&reply[83], &message[87], 8);
			}
#ifndef UNRESPONSIVE
			//send the message
			send_message(flow.dst_ip, reply);
#endif

		}
	}
}

void Aitf_Manager::send_message(uint32_t ip, std::vector<uint8_t> message){
	
	
	//convert the internet address to a string
	std::string ip_addr = Helpers::ip_to_string(ip);
	log(logDEBUG) << "Sending message to " << ip_addr;

	//prepare the socket
	boost::asio::ip::udp::resolver resolver(timeout_io);
	boost::asio::ip::udp::socket socket(timeout_io);
	socket.open(boost::asio::ip::udp::v4());
	boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), ip_addr,"50000");
	boost::asio::ip::udp::endpoint receiver_endpoint = *resolver.resolve(query);

	//send the message
	socket.send_to(boost::asio::buffer(message), receiver_endpoint);
}

void Aitf_Manager::handle_filter_request(std::vector<uint8_t> message){
	log(logINFO) << "Received filter request";

	//check the message length
	if(message.size() == 83){

		//pull out the dst_ip
		uint32_t dst_ip;
		memcpy(&dst_ip, &message[DST_REQ_INDEX], 4);

		//if the victim is within its rate limit
		if(aitf_hosts_table->check_from_rate(dst_ip)){

			Flow flow(std::vector<uint8_t>(&message[1], &message[1]+81));
			/*for(int i = 0; i < 83; i++){
			  log(logDEBUG2) << (int) message[i];
			  }*/
			//apply temp filter
			filter_table->add_temp_filter(flow);

			log(logDEBUG2) << "checking shadow table";
			//check shadow table
			int request_attempts = shadow_table->attempt_count(flow);

			//if there is only one gateway in the list, then this gateway
			//must deal with it
			if(flow.pointer == 0){
				deal_with_attacker(flow, request_attempts);
			}
			else{
				log(logINFO) << "Dealing with another gateway";
				//if this is a repeat offense for the gateway (twice)
				if(request_attempts >= 2){
					attempt_escalation(flow, request_attempts - 1);
				}
				else{

					//create a new flow with only the atk gtw
					//MOVED TO THE RECEIVER
					//Flow ammended_flow;
					//ammended_flow.src_ip = flow.src_ip;
					//ammended_flow.gtw0_ip = flow.gtw0_ip;
					//ammended_flow.gtw0_rvalue = flow.gtw0_rvalue;
					//ammended_flow.dst_ip = flow.dst_ip;
					//ammended_flow.pointer = 0;

					std::vector<uint8_t> handshake = create_handshake(flow, 0);

					//Add filter for returning handshake with src as gtw0_ip
					Flow handshake_flow(flow);
					handshake_flow.src_ip = handshake_flow.gtw0_ip;
					filter_table->add_temp_filter(handshake_flow);

					log(logDEBUG2) << "Handshake filter src: " << Helpers::ip_to_string(handshake_flow.src_ip);
					log(logDEBUG2) << "Handshake filter dst: " << Helpers::ip_to_string(handshake_flow.dst_ip);
					for(int i = 0; i <= flow.pointer; i++){
						log(logDEBUG2) << "gtw ip " << i << " "<< Helpers::ip_to_string(handshake_flow.get_gtw_ip_at(i));
						log(logDEBUG2) << "gtw rvalue " << i << " "<< handshake_flow.get_gtw_rvalue_at(i);
					}

					log(logDEBUG2) << "About to send 1handshake with:";
					log(logDEBUG2) << "gtw0 rvalue of: " << flow.gtw0_rvalue;

					//Send the handshake
					send_message(flow.gtw0_ip, handshake);

					//add callback to detect timedout handshakes
					uint8_t escalation = message[message.size()-1];
					boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(timeout_io, boost::posix_time::seconds(TEMP_TIME)));
					timer->async_wait(boost::bind(&Aitf_Manager::unresponsive_gateway, this, boost::asio::placeholders::error, timer, flow, escalation, 0));

				}
			}//else flow pointer != 0
		}//if rate ok
	}//if size ok
	else{
		log(logWARNING) << "Message size is incorrect!!: " << message.size();
	}
}

void Aitf_Manager::deal_with_attacker(Flow flow, int request_attempts){
	log(logINFO) << "dealing with attacker";

	//if the host is AITF enabled
	if(aitf_hosts_table->contains_host(flow.src_ip)){
		//if this is a repeat offense
		if(request_attempts  > 0){
			log(logINFO) << "ATTACKER WAS LYING filtering locally";
			filter_table->add_long_filter(flow);
		}
		else{
			std::vector<uint8_t> message(9);
			message[0] = 4;
			memcpy(&message[1], &flow.src_ip, 4);
			memcpy(&message[5], &flow.dst_ip, 4);

			uint32_t* ipINT = (uint32_t*) &message[1];
			
			log(logDEBUG2) << "sending this ip " << Helpers::ip_to_string(*ipINT);

			//contact host
			send_message(flow.src_ip, message);
			//send udp packet
			boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(timeout_io, boost::posix_time::seconds(TEMP_TIME)));
			timer->async_wait(boost::bind(&Aitf_Manager::unresponsive_host, this, boost::asio::placeholders::error, timer, flow));
		}
	}
	else{
		log(logINFO) << "Non aitf host. filtering locally";
		//For non AITF enabled hosts, just cut them off
		filter_table->add_long_filter(flow);

	}
}

std::vector<uint8_t> Aitf_Manager::create_handshake(Flow flow, int attempts){
	log(logDEBUG) << "Creating handshake";
	//generate the nonce
	uint64_t nonce = Hasher::hash(*gateway_key, (unsigned char*) &flow.to_byte_vector()[0], 81);
	//construct the handshake request wiht the flow
	//and prepend the source IP and msg_type and the nonce at the end
	std::vector<uint8_t> handshake(95);
	handshake[0] = 1;
	memcpy(&handshake[1], &MY_IP, 4);
	handshake[5] = (uint8_t) attempts;
	memcpy(&handshake[6], &flow.to_byte_vector()[0], 81);
	memcpy(&handshake[87], &nonce, 8);
	log(logDEBUG2) << "Finished creating handshake";

	return handshake;
}

void Aitf_Manager::unresponsive_host(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, Flow flow){
	log(logINFO) << "_________________In unresponsive_host callback_________________";
	
		log(logDEBUG2) << Helpers::ip_to_string(flow.src_ip);

		log(logDEBUG2) << Helpers::ip_to_string(flow.gtw0_ip);

		log(logDEBUG2) << flow.gtw0_rvalue;

		log(logDEBUG2) << Helpers::ip_to_string(flow.dst_ip);
	//if the flow has not been added to the table then cut off the client
	if(shadow_table->attempt_count(flow) == 0){
		log(logINFO) << "HOST WAS UNRESPONSIVE";
		filter_table->add_long_filter(flow);
	}
	else{
		log(logINFO) << "HOST WAS RESPONSIVE";
	}

}

void Aitf_Manager::unresponsive_gateway(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, Flow flow, uint8_t escalation, int attempts){
	log(logINFO) << "_________________In unresponsive_gateway callback_________________";

	//if the flow has not been added to the table then check for escalation
	if(shadow_table->attempt_count(flow) == 0){
		log(logINFO) << "GATEWAY WAS UNRESPONSIVE";
		if(escalation == 1){
			//if there is escalation attempt to escalate to the next gateway
			attempt_escalation(flow, attempts);

		}
		else{
			//if no escalation block locally
			filter_table->add_long_filter(flow);
		}
	}
	else{
		log(logINFO) << "GATEWAY WAS RESPONSIVE";
		//increment the shadowtable filter count to show the number of attempts made
		for(int i = 0; i < attempts; i++){
			shadow_table->add_long_filter(flow);
		}
	}

}

void Aitf_Manager::attempt_escalation(Flow flow, int attempts){
	log(logINFO) << "Attempting Escalation";
	//if there is an available gateway in the flow
	if(attempts < flow.pointer){

		

		//Create filter for return handshake with only correct gateways
		uint32_t dst_gtw_ip = flow.get_gtw_ip_at(attempts);
		Flow handshake_flow;
		//set the src and dst
		handshake_flow.src_ip = dst_gtw_ip;
		handshake_flow.dst_ip = flow.dst_ip;
		for(int i = attempts, j = 0; i <= flow.pointer; i++, j++){
			uint32_t ip = flow.get_gtw_ip_at(i);
			uint64_t rvalue = flow.get_gtw_rvalue_at(i);
			handshake_flow.set_gtw_ip_at(j, ip);
			handshake_flow.set_gtw_rvalue_at(j, rvalue);
			handshake_flow.pointer = j;
		}

		filter_table->add_temp_filter(handshake_flow);

		//set the src to 0 for *
		Flow modified_flow(flow);
		modified_flow.src_ip = 0;

		//construct the handshake
		std::vector<uint8_t> handshake = create_handshake(modified_flow, attempts);

		//SEND HANDSHAKE
		send_message(dst_gtw_ip, handshake);
		//add callback for timedout connections
		boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(timeout_io, boost::posix_time::seconds(TEMP_TIME)));
		timer->async_wait(boost::bind(&Aitf_Manager::unresponsive_gateway, this, boost::asio::placeholders::error, timer, flow, 1, attempts+1));
	}
	else{
		//no other option but to block locally
		filter_table->add_long_filter(flow);
	}
}
