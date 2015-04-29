all: SimpleSend SimpleReceive

SimpleSend: SimpleSend.cpp
	g++ -g SimpleSend.cpp -lboost_filesystem -lboost_system -lboost_serialization -lboost_thread -lpthread -o SimpleSend.out

SimpleReceive: SimpleReceive.cpp
	g++ -g SimpleReceive.cpp -lboost_filesystem -lboost_system -lboost_serialization -lboost_thread -lpthread -o SimpleReceive.out

clean:
	rm -rf *.out *.o