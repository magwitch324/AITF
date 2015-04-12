all: Gateway

Gateway: GatewayMain.cpp Aitf_Manager.cpp Aitf_Manager.hpp Udp_Server.cpp Udp_Server.hpp logger.hpp
	g++ -g GatewayMain.cpp Aitf_Manager.cpp Udp_Server.cpp -lboost_filesystem -lboost_system -lboost_serialization -lboost_thread -lpthread -o Gateway.out

SimpleSend: SimpleSend.cpp
	g++ -g SimpleSend.cpp -lboost_filesystem -lboost_system -lboost_serialization -lboost_thread -lpthread -o SimpleSend.out

clean:
	rm -rf *.out *.o