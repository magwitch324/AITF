#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>


using namespace boost::asio;
using boost::asio::ip::udp;

boost::array<uint8_t, 165> recv_buff;
boost::asio::io_service udp_io;
boost::asio::ip::udp::socket* udp_sock = NULL;
boost::asio::ip::udp::endpoint remote_endpoint_;

void listen();

class Udp_Server{
  public:
  static void handle_receive(const boost::system::error_code& error,
    std::size_t size){
  std::cout << "-------------------Receiving message-------------------\n";

  for(int i = 0; i < 10; i++){
   std::cout << (int) recv_buff[i] << "\n";
  }

  listen();



}
};

void listen(){
   udp_sock->async_receive_from(buffer(recv_buff), remote_endpoint_,
      boost::bind(&Udp_Server::handle_receive, 
        placeholders::error, placeholders::bytes_transferred));
}

int main(){
  std::cout << "Starting udp server";

  //create the socket
  udp::socket test(udp_io, udp::endpoint(udp::v4(), 50000));
  udp_sock = &test;

  //start async listening
    listen();

  udp_io.run();
}

