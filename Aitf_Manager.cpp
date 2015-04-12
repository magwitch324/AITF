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

	void Aitf_Manager::join(){
		log(logINFO) << "Stopping aitf manager";
		server->stop();
		delete(server);
		aitf_thread.join();
		log(logINFO) << "Aitf manager stopped";
	}


	void Aitf_Manager::packet_arrived(uint8_t msg_type, std::vector<uint8_t> recv_buf){

	}