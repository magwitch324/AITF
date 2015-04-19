#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "../logger.hpp"
#include "Aitf_Manager.hpp"
#include "Udp_Server.hpp"

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
		memcpy(&dst_ip, &message[DST_REQ_INDEX], 4);

		//if the victim is within its rate limit
		if(aitf_hosts_table->check_from_rate(dst_ip)){

			uint8_t flow[81];
			memcpy(&flow, &message[1], 81);
			//apply temp filter
			filter_table->add_temp_filter(flow);

			//check shadow table
			bool repeat_request = true;// shadow_table->check_request(flow);

			//check route length
			uint8_t flow_ptr = flow[4];

			//if there is only one gateway in the list, then this gateway
			//must deal with it
			if(flow_ptr == 0){
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

						boost::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(timeout_io, boost::posix_time::seconds(TEMP_TIME)));
						timer->async_wait(boost::bind(&Aitf_Manager::unresponsive_host, this, boost::asio::placeholders::error, timer, flow));
					}
				}
			}
			else{
				//insert gateway filter for response

				//send handshake
			}

			//add shadow filter


		}
	}
	else{
		log(logWARNING) << "Message size is incorrect!!: " << message.size();
	}
}

void Aitf_Manager::unresponsive_host(const boost::system::error_code& e, boost::shared_ptr<boost::asio::deadline_timer> timer, uint8_t flow[]){
	//TODO check shadow table that the flow is now there
	//if it isnt disconnect the host
	filter_table->add_long_filter(flow);
}
