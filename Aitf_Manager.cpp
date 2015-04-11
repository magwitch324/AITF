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

		aitf_thread = boost::thread(&Aitf_Manager::run, this);

	}

	void Aitf_Manager::join(){
		server->stop();
		aitf_thread.join();
	}


	void Aitf_Manager::packet_arrived(uint8_t msg_type, std::vector<uint8_t> recv_buf){

	}