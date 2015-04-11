#ifndef AITF_MANAGER_HPP
#define AITF_MANAGER_HPP
#include "Packet_Listener.hpp"
#include "Udp_Server.hpp"

class Aitf_Manager : public Packet_Listener
{
public:
  Aitf_Manager();
  void start_thread();
  void join();
  virtual void packet_arrived(uint8_t msg_type, std::vector<uint8_t> recv_buf);

private:
	void run();
	boost::thread aitf_thread;
	Udp_Server* server;

};

#endif