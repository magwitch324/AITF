all: Gateway

SOURCE = ./Aitf_Manager/*.cpp  ./Tables/*.cpp ./*.cpp

Gateway: ./Aitf_Manager/*.cpp *.hpp
	g++ -g -std=gnu++11 $(SOURCE) -lboost_filesystem -lboost_system -lboost_serialization -lboost_thread -lpthread -o Gateway.out

clean:
	rm -rf *.out *.o