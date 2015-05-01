all: Gateway

SOURCE = ./Aitf_Manager/*.cpp  ./Tables/*.cpp ./*.cpp ./Internet_Manager/*.cpp

Gateway: ./Aitf_Manager/*.cpp ./Internet_Manager/*.cpp *.hpp
	g++ -g -std=gnu++11 $(SOURCE) -lcrypto -lboost_system -lboost_thread -lpthread -lnfnetlink -lnetfilter_queue -o Gateway.out

Lying: $(SOURCE)
	g++ -g -std=gnu++11 -DLYING $(SOURCE) -lcrypto -lboost_system -lboost_thread -lpthread -lnfnetlink -lnetfilter_queue -o LGateway.out	

clean:
	rm -rf *.out *.o