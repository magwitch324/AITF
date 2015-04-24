all: Host

SOURCE = ./*.cpp

Host: ./*.cpp ./*.hpp
	g++ -g -std=gnu++11 $(SOURCE) -lboost_filesystem -lboost_system -lboost_serialization -lboost_thread -lpthread -o Host.out

Clean:
