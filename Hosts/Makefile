SOURCE = ./Managers/*.cpp ./Modules/*.cpp ./Servers/*.cpp ./Tables/*.cpp ./*.cpp 

#-lboost_filesystem -lboost_system -lboost_serialization -lboost_thread -lpthread -lnfnetlink -lnetfilter_queue

all: Normal

Normal: $(SOURCE) 
	g++ -g -std=gnu++11 $(SOURCE) -o Host.out -lboost_system -lboost_thread -lpthread -lnfnetlink -lnetfilter_queue
	
lying: $(SOURCE) 
	g++ -g -std=gnu++11 -DLYING_ATTACKER $(SOURCE) -o Lying.out -lboost_system -lboost_thread -lpthread -lnfnetlink -lnetfilter_queue
	
long: $(SOURCE) 
	g++ -g -std=gnu++11 -DT_LONG=60 $(SOURCE) -o Long_Host.out -lboost_system -lboost_thread -lpthread -lnfnetlink -lnetfilter_queue

clean:
	rm -rf *.out *.o
