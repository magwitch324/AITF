#include <netinet/ip.h>
#include <linux/udp.h>
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


uint16_t ip_checksum(void* vdata, size_t length) {
	// Cast the data pointer to one that can be indexed.
	char* data=(char*)vdata;

	// Initialise the accumulator.
	uint32_t acc=0xffff;

	// Handle complete 16-bit blocks.
	for (size_t i=0;i+1<length;i+=2) {
		uint16_t word;
		memcpy(&word,data+i,2);
		acc+=ntohs(word);
		if (acc>0xffff) {
			acc-=0xffff;
		}
	}

	// Handle any partial block at the end of the data.
	if (length&1) {
		uint16_t word=0;
		memcpy(&word,data+length-1,1);
		acc+=ntohs(word);
		if (acc>0xffff) {
			acc-=0xffff;
		}
	}

	// Return the checksum in network byte order.
	return htons(~acc);
}

unsigned short checksum1(const unsigned char *buf, unsigned size)
{
	unsigned sum = 0;
	int i;

	/* Accumulate checksum */
	for (i = 0; i < size - 1; i += 2)
	{
		unsigned short word16 = *(unsigned short *) &buf[i];
		sum += word16;
	}

	/* Handle odd-sized case */
	if (size & 1)
	{
		unsigned short word16 = (unsigned char) buf[i];
		sum += word16;
	}

	/* Fold to get the ones-complement result */
	while (sum >> 16) sum = (sum & 0xFFFF)+(sum >> 16);

	/* Invert to get the negative in ones-complement arithmetic */
	return htons(~sum);
}

static unsigned short compute_checksum(unsigned short *addr, unsigned int count) {
  register unsigned long sum = 0;
  while (count > 1) {
    sum += * addr++;
    count -= 2;
  }
  //if any bytes left, pad the bytes and add
  if(count > 0) {
    sum += ((*addr)&htons(0xFF00));
  }
  //Fold sum to 16 bits: add carrier to result
  while (sum>>16) {
  	sum = (sum & 0xffff) + (sum >> 16);
  }
  //one's complement
  sum = ~sum;
  return ((unsigned short)sum);
}

/* set ip checksum of a given ip header*/
void compute_ip_checksum(struct iphdr* iphdrp){
  iphdrp->check = 0;
  iphdrp->check = compute_checksum((unsigned short*)iphdrp, iphdrp->ihl<<2);
}


int Packet_Manager::packet_callback(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfad, void *data){
	log(logINFO) << "----------------ITS HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!----------------";

    struct nfqnl_msg_packet_hdr *ph;
    int len;
    unsigned char *ORIGINAL_DATA;
    struct iphdr *ipHeader;

    //get the packet id
    ph = nfq_get_msg_packet_hdr(nfad);
    u_int32_t id = ntohl(ph->packet_id);

    //get the packet contents
    len = nfq_get_payload(nfad, &ORIGINAL_DATA);
    //payloadData = (unsigned char*) malloc(len);
    //memcpy(payloadData, ORIGINAL_DATA, len);


    log(logDEBUG) << "packet len: " << len;
    ipHeader = (struct iphdr *)ORIGINAL_DATA;
    
    //DEBUG CHECKS
    ipCheck = ipHeader->check;
    log(logDEBUG2) << std::hex << ipHeader->check;


    if(ipHeader->protocol == IPPROTO_UDP){
    	log(logDEBUG) << "Udp Packet";

    	unsigned char modified_packet[len+81];
    	memcpy(&modified_packet[0], ORIGINAL_DATA, sizeof(*ipHeader));
    	memcpy(&modified_packet[sizeof(*ipHeader) + 81], ORIGINAL_DATA + sizeof(*ipHeader), len - sizeof(*ipHeader));
    	((struct iphdr*) &modified_packet[0])->protocol = 143;
    	((struct iphdr*) &modified_packet[0])->tot_len = htons(len+81);
    	
    	//compute the new checksum
    	compute_ip_checksum((struct iphdr*) &modified_packet[0]);
    	log(logDEBUG2) << std::hex << ((struct iphdr*) &modified_packet[0])->check;
    	return nfq_set_verdict(qh, id, NF_ACCEPT, len+81, &modified_packet[0]);
    }
    else if(ipHeader->protocol == 143){
    	log(logDEBUG) << "AITF PACKET!!!!";

    	unsigned char modified_packet[len-81];
    	memcpy(&modified_packet[0], ORIGINAL_DATA, sizeof(*ipHeader));
    	memcpy(&modified_packet[sizeof(*ipHeader)], ORIGINAL_DATA + sizeof(*ipHeader) + 81, len - sizeof(*ipHeader) - 81);
    	((struct iphdr*) &modified_packet[0])->protocol = 17;
    	((struct iphdr*) &modified_packet[0])->tot_len = htons(len-81);
    	
    	//compute the new checksum
    	compute_ip_checksum((struct iphdr*) &modified_packet[0]);
    	log(logDEBUG2) << std::hex << ((struct iphdr*) &modified_packet[0])->check;
    	return nfq_set_verdict(qh, id, NF_ACCEPT, len-81, &modified_packet[0]);
    }
    
    //issue a verdict on a packet
    //qh: netfilter queue handle; id: ID assigned to packet by netfilter; verdict: verdict to return to netfilter, data_len: number
    //of bytes of data pointed by buf, buf: the buffer that contains the packet data (payload)
// return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
    return nfq_set_verdict(qh, id, NF_ACCEPT, len, ORIGINAL_DATA);
}




Packet_Manager::~Packet_Manager(void) {
	log(logINFO) << "Ending Packet_Manager";
	this->state.store(this->ENDING, boost::memory_order_relaxed);
	log(logINFO) << "Ended Packet_Manager";
}
