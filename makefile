all: SimpleSend

SimpleSend: SimpleSend.cpp
	g++ -g SimpleSend.cpp -lboost_filesystem -lboost_system -lboost_serialization -lboost_thread -lpthread -o SimpleSend.out

clean:
	rm -rf *.out *.o