#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "logger.hpp"
#include "Aitf_Manager.hpp"
#include "Udp_Server.hpp"
	
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
		aitf_thread.join();
		log(logINFO) << "Aitf manager stopped";
	}


	void Aitf_Manager::packet_arrived(uint8_t msg_type, std::vector<uint8_t> recv_buf){
		switch(msg_type){
			case 0: 
				handle_request(recv_buf);
			break;
			case 1:
			break;
			case 2:
			break;
		}
	}

	void Aitf_Manager::handle_request(std::vector<uint8_t> message){
		
		//pull out the dst_ip
		uint32_t dst_ip;
		memcpy(&dst_ip, &message[130], 4);

		//if the victim is within its rate limit
		if(hosts_table.check_from_rate(dst_ip)){
		
		//apply temp filter
		//check shadow table

		//check route length

		//if 1 handle attacker

		//if more

		//insert gateway filter for response

		//send handshake
		}
	}