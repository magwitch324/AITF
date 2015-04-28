#include "Packet_Manager.hpp"
#include "../logger.hpp"

Packet_Manager::Packet_Manager(Packet_Sniffer* listener_in){
	log(logINFO) << "Creating Packet_Manager";
	listener = listener_in;

	my_netfilterqueue_handle = nfq_open();
	nfq_unbind_pf(my_netfilterqueue_handle, AF_INET);
	nfq_bind_pf(my_netfilterqueue_handle, AF_INET);

	//Setup the state and queue
	state.store(STARTING, boost::memory_order_relaxed);

	internet_queue_handle = nfq_create_queue(my_netfilterqueue_handle, 1, &(Packet_Manager::packet_callback), NULL);
	nfq_set_mode(internet_queue_handle, NFQNL_COPY_PACKET, 0xffff);

	state.store(STARTED, boost::memory_order_relaxed);

	//this->victim_manager = new VictimManager(this->my_netfilterqueue_handle, input_queue_num);
	//this->attack_manager = new AttackManager(this->my_netfilterqueue_handle, output_queue_num);
}

void Packet_Manager::start(){

	log(logINFO) << "Starting Packet_Manager";
	int fd;
	int rv;
	char buf[4096] __attribute__ ((aligned));

	fd_set readset;
	struct timeval tv;

	fd = nfq_fd(my_netfilterqueue_handle);

	while (ENDING != state.load(boost::memory_order_relaxed)) {
		FD_ZERO(&readset);
		FD_SET(fd, &readset);
		tv.tv_sec = 0;
		tv.tv_usec = 500;
		if (select(fd+1, &readset, NULL, NULL, &tv) <= 0 ) {
			continue;
		}

		rv = recv(fd, buf, sizeof(buf), 0);//MSG_DONTWAIT );
		if (rv < 0) {
			continue;
		}
		log(logINFO) << "Internet Queue is Receiceiving Packet";
		nfq_handle_packet(my_netfilterqueue_handle, buf, rv);
	}
}

void Packet_Manager::stop(){
	state.store(ENDING, boost::memory_order_relaxed);
	nfq_destroy_queue( internet_queue_handle );
	nfq_close(my_netfilterqueue_handle);
}

uint32_t Packet_Manager::print_pkt (struct nfq_data *tb) {
	int id = 0;
	struct nfqnl_msg_packet_hdr *ph;

	ph = nfq_get_msg_packet_hdr(tb);
	if (ph) {
		id = ntohl(ph->packet_id);
	}

	return id;
}

int Packet_Manager::packet_callback(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data){
	log(logINFO) << "ITS HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1";
	uint32_t id = print_pkt(nfad);
	return nfq_set_verdict(qh, id, NF_DROP, 0, NULL);
}




Packet_Manager::~Packet_Manager(void) {
	log(logINFO) << "Ending Packet_Manager";
	this->state.store(this->ENDING, boost::memory_order_relaxed);
	log(logINFO) << "Ended Packet_Manager";
}
