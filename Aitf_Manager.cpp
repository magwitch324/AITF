#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "logger.hpp"
#include "Aitf_Manager.hpp"
	
	Aitf_Manager::Aitf_Manager(){

	}

	void Aitf_Manager::run(){
		boost::posix_time::seconds work_time(10);
		log(logINFO) << "THIS IS A TEST";
		boost::this_thread::sleep(work_time);
	}
	
	void Aitf_Manager::start_thread(){

		aitf_thread = boost::thread(&Aitf_Manager::run, this);

	}

	void Aitf_Manager::join(){
		aitf_thread.join();
	}