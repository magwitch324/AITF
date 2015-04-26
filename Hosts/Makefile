SOURCE = ./Managers/*.cpp  ./Tables/*.cpp ./*.cpp


all: Host

Host: $(SOURCE) 
	cd QueuingModule
	make
	cd ..
	
	g++ -g -std=gnu++11 $(SOURCE) -lboost_filesystem -lboost_system -lboost_serialization -lboost_thread -lpthread -o Host.out

clean:
	cd QueuingModule
	make clean
	cd ..
	
	rm -rf *.out *.o