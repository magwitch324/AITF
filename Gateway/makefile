all: Gateway

Gateway: *.cpp *.hpp
	g++ -g -std=gnu++11 *.cpp -lboost_filesystem -lboost_system -lboost_serialization -lboost_thread -lpthread -o Gateway.out

clean:
	rm -rf *.out *.o