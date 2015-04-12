#include <boost/asio.hpp>
#include <boost/thread.hpp>

using namespace boost::asio;
using boost::asio::ip::udp;

int main(){
	boost::asio::io_service io_service;
udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(),"127.0.0.1","13");
	udp::endpoint receiver_endpoint = *resolver.resolve(query);

	udp::socket socket(io_service);
	socket.open(udp::v4());

	boost::shared_ptr<std::string> message(new std::string("Transfer"));

	socket.send_to(boost::asio::buffer(*message), receiver_endpoint);
}
