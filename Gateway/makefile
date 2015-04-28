all: Gateway

SOURCE = ./Aitf_Manager/*.cpp  ./Tables/*.cpp ./*.cpp ./Internet_Manager/*.cpp

Gateway: ./Aitf_Manager/*.cpp ./Internet_Manager/*.cpp *.hpp
	g++ -g -std=gnu++11 $(SOURCE) -lcrypto -lboost_filesystem -lboost_system -lboost_serialization -lboost_thread -lpthread -lnfnetlink -lnetfilter_queue -o Gateway.out

Internet: ./Internet_Manager/*.cpp *.hpp
	g++ -g -std=gnu++11 ./*.cpp ./Internet_Manager/*.cpp -lcrypto -lboost_filesystem -lboost_system -lboost_serialization -lboost_thread -lpthread -lnfnetlink -lnetfilter_queue -o Gateway.out

clean:
	rm -rf *.out *.o