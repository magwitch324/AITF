all: Gateway

Gateway: GatewayMain.cpp Aitf_Manager.cpp Aitf_Manager.hpp logger.hpp
	g++ -g GatewayMain.cpp Aitf_Manager.cpp -lboost_filesystem -lboost_system -lboost_serialization -lboost_thread -lpthread -o Gateway.out

clean:
	rm -rf *.out *.o