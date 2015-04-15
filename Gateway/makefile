all: Gateway

Gateway: GatewayMain.cpp Aitf_Manager.cpp Aitf_Manager.hpp Udp_Server.cpp Udp_Server.hpp Aitf_Hosts_Table.hpp Aitf_Hosts_Table.cpp logger.hpp
	g++ -g -std=gnu++11 GatewayMain.cpp Aitf_Manager.cpp Udp_Server.cpp Aitf_Hosts_Table.cpp -lboost_filesystem -lboost_system -lboost_serialization -lboost_thread -lpthread -o Gateway.out

clean:
	rm -rf *.out *.o