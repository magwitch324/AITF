SOURCE = ./Managers/*.cpp ./Modules/*.cpp ./Tables/*.cpp ./*.cpp 

#-lboost_filesystem -lboost_system -lboost_serialization -lboost_thread -lpthread -lnfnetlink -lnetfilter_queue

all: Host

Host: $(SOURCE) 
	g++ -g -std=gnu++11 $(SOURCE) -o Host.out -lboost_filesystem -lboost_system -lboost_serialization -lboost_thread -lpthread -lnfnetlink -lnetfilter_queue

clean:
	rm -rf *.out *.o
