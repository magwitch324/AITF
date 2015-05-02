/*
 * PolicyModule.cpp
 */


 #include <netinet/in.h>
 #include <arpa/inet.h>

#include "PolicyModule.hpp"
#include "../logger.hpp"
#include "../Helpers.hpp"


PolicyModule::PolicyModule(){
	llog(logDEBUG) << "Creating policy module";

	bandwidthUsage = new Async_Auto_Flow_Table( "./hostBandwidth.log", 500);
	//bandwidthUsage = new Async_Auto_Flow_Table();

	defaults[inet_addr("10.4.13.1")] = 5000;

	defaults[inet_addr("10.4.13.128")] = 5000;
	defaults[inet_addr("10.4.13.129")] = 5000;
	defaults[inet_addr("10.4.13.130")] = 5000;
	defaults[inet_addr("10.4.13.131")] = 5000;
	defaults[inet_addr("10.4.13.132")] = 5000;
	defaults[inet_addr("10.4.13.133")] = 5000;
	defaults[inet_addr("10.4.13.134")] = 5000;
	defaults[inet_addr("10.4.13.135")] = 5000;
	defaults[inet_addr("10.4.13.136")] = 5000;
	defaults[inet_addr("10.4.13.137")] = 5000;
	defaults[inet_addr("10.4.13.138")] = 5000;
	defaults[inet_addr("10.4.13.139")] = 5000;
	defaults[inet_addr("10.4.13.140")] = 5000;
	defaults[inet_addr("10.4.13.141")] = 5000;
	defaults[inet_addr("10.4.13.142")] = 5000;
	defaults[inet_addr("10.4.13.143")] = 5000;
	defaults[inet_addr("10.4.13.144")] = 5000;
	defaults[inet_addr("10.4.13.145")] = 5000;
	defaults[inet_addr("10.4.13.146")] = 5000;
	defaults[inet_addr("10.4.13.147")] = 5000;
	defaults[inet_addr("10.4.13.148")] = 5000;
	defaults[inet_addr("10.4.13.149")] = 5000;
	defaults[inet_addr("10.4.13.150")] = 5000;
	defaults[inet_addr("10.4.13.151")] = 5000;
	defaults[inet_addr("10.4.13.152")] = 5000;
	defaults[inet_addr("10.4.13.153")] = 5000;
	defaults[inet_addr("10.4.13.154")] = 5000;
	defaults[inet_addr("10.4.13.155")] = 5000;
	defaults[inet_addr("10.4.13.156")] = 5000;
	defaults[inet_addr("10.4.13.157")] = 5000;
	defaults[inet_addr("10.4.13.158")] = 5000;
	defaults[inet_addr("10.4.13.159")] = 5000;
	defaults[inet_addr("10.4.13.160")] = 5000;
	defaults[inet_addr("10.4.13.161")] = 5000;
	defaults[inet_addr("10.4.13.162")] = 5000;
	defaults[inet_addr("10.4.13.163")] = 5000;
	defaults[inet_addr("10.4.13.164")] = 5000;
	defaults[inet_addr("10.4.13.165")] = 5000;
	defaults[inet_addr("10.4.13.166")] = 5000;
	defaults[inet_addr("10.4.13.167")] = 5000;
	defaults[inet_addr("10.4.13.168")] = 5000;
	defaults[inet_addr("10.4.13.169")] = 5000;
	defaults[inet_addr("10.4.13.170")] = 5000;
	defaults[inet_addr("10.4.13.171")] = 5000;
	defaults[inet_addr("10.4.13.172")] = 5000;
	defaults[inet_addr("10.4.13.173")] = 5000;
	defaults[inet_addr("10.4.13.174")] = 5000;
	defaults[inet_addr("10.4.13.175")] = 5000;
	defaults[inet_addr("10.4.13.176")] = 5000;
	defaults[inet_addr("10.4.13.177")] = 5000;
	defaults[inet_addr("10.4.13.178")] = 5000;
	defaults[inet_addr("10.4.13.179")] = 5000;
	defaults[inet_addr("10.4.13.180")] = 5000;
	defaults[inet_addr("10.4.13.181")] = 5000;
	defaults[inet_addr("10.4.13.182")] = 5000;
	defaults[inet_addr("10.4.13.183")] = 5000;
	defaults[inet_addr("10.4.13.184")] = 5000;
	defaults[inet_addr("10.4.13.185")] = 5000;
	defaults[inet_addr("10.4.13.186")] = 5000;
	defaults[inet_addr("10.4.13.187")] = 5000;
	defaults[inet_addr("10.4.13.188")] = 5000;
	defaults[inet_addr("10.4.13.189")] = 5000;
	defaults[inet_addr("10.4.13.190")] = 5000;
	defaults[inet_addr("10.4.13.191")] = 5000;
	defaults[inet_addr("10.4.13.192")] = 5000;
	defaults[inet_addr("10.4.13.193")] = 5000;
	defaults[inet_addr("10.4.13.194")] = 5000;
	defaults[inet_addr("10.4.13.195")] = 5000;
	defaults[inet_addr("10.4.13.196")] = 5000;
	defaults[inet_addr("10.4.13.197")] = 5000;
	defaults[inet_addr("10.4.13.198")] = 5000;
	defaults[inet_addr("10.4.13.199")] = 5000;
	defaults[inet_addr("10.4.13.200")] = 5000;
	defaults[inet_addr("10.4.13.201")] = 5000;
	defaults[inet_addr("10.4.13.202")] = 5000;
	defaults[inet_addr("10.4.13.203")] = 5000;
	defaults[inet_addr("10.4.13.204")] = 5000;
	defaults[inet_addr("10.4.13.205")] = 5000;
	defaults[inet_addr("10.4.13.206")] = 5000;
	defaults[inet_addr("10.4.13.207")] = 5000;
	defaults[inet_addr("10.4.13.208")] = 5000;
	defaults[inet_addr("10.4.13.209")] = 5000;
	defaults[inet_addr("10.4.13.210")] = 5000;
	defaults[inet_addr("10.4.13.211")] = 5000;
	defaults[inet_addr("10.4.13.212")] = 5000;
	defaults[inet_addr("10.4.13.213")] = 5000;
	defaults[inet_addr("10.4.13.214")] = 5000;
	defaults[inet_addr("10.4.13.215")] = 5000;
	defaults[inet_addr("10.4.13.216")] = 5000;
	defaults[inet_addr("10.4.13.217")] = 5000;
	defaults[inet_addr("10.4.13.218")] = 5000;
	defaults[inet_addr("10.4.13.219")] = 5000;
	defaults[inet_addr("10.4.13.220")] = 5000;
	defaults[inet_addr("10.4.13.221")] = 5000;
	defaults[inet_addr("10.4.13.222")] = 5000;
	defaults[inet_addr("10.4.13.223")] = 5000;
	defaults[inet_addr("10.4.13.224")] = 5000;
	defaults[inet_addr("10.4.13.225")] = 5000;
	defaults[inet_addr("10.4.13.226")] = 5000;
	defaults[inet_addr("10.4.13.227")] = 5000;
	defaults[inet_addr("10.4.13.228")] = 5000;
	defaults[inet_addr("10.4.13.229")] = 5000;
	defaults[inet_addr("10.4.13.230")] = 5000;
	defaults[inet_addr("10.4.13.231")] = 5000;
	defaults[inet_addr("10.4.13.232")] = 5000;
	defaults[inet_addr("10.4.13.233")] = 5000;
	defaults[inet_addr("10.4.13.234")] = 5000;
	defaults[inet_addr("10.4.13.235")] = 5000;
	defaults[inet_addr("10.4.13.236")] = 5000;
	defaults[inet_addr("10.4.13.237")] = 5000;
	defaults[inet_addr("10.4.13.238")] = 5000;
	defaults[inet_addr("10.4.13.239")] = 5000;
	defaults[inet_addr("10.4.13.240")] = 5000;
	defaults[inet_addr("10.4.13.241")] = 5000;
	defaults[inet_addr("10.4.13.242")] = 5000;
	defaults[inet_addr("10.4.13.243")] = 5000;
	defaults[inet_addr("10.4.13.244")] = 5000;
	defaults[inet_addr("10.4.13.245")] = 5000;
	defaults[inet_addr("10.4.13.246")] = 5000;
	defaults[inet_addr("10.4.13.247")] = 5000;
	defaults[inet_addr("10.4.13.248")] = 5000;
	defaults[inet_addr("10.4.13.249")] = 5000;
	defaults[inet_addr("10.4.13.250")] = 5000;
	defaults[inet_addr("10.4.13.251")] = 5000;
	defaults[inet_addr("10.4.13.252")] = 5000;
	defaults[inet_addr("10.4.13.253")] = 5000;
	defaults[inet_addr("10.4.13.254")] = 5000;
	defaults[inet_addr("10.4.13.255")] = 5000;
	defaults[inet_addr("10.4.13.256")] = 5000;

	bandwidthUsage->start_thread();
}


PolicyModule::~PolicyModule() {
	llog(logDEBUG) << "Destroying policy module";
	bandwidthUsage->stop_thread();
	delete bandwidthUsage;
}
/*
 * receivedPacket: determines what should happen based on the source ip and the size of the packet
 * 	input: 	flow - the flow that is going to be cataloged
 * 			size - the size of the packet that was received
 * 	output: size - everything works out and nothing should be done
 * 			-1 - the source exceeded it's limit for the first time
 */
int PolicyModule::receivedPacket(Flow flow, int size){

	int max = 0, bwu_ret;

	if (defaults.count(flow.src_ip) > 0) {
		llog(logDEBUG2) << "Found max " << Helpers::ip_to_string(flow.src_ip) << " for " << defaults[flow.src_ip];
		max = defaults[flow.src_ip];
	} else {
		max = -1;
	}

	bwu_ret = bandwidthUsage->addValue(flow, size, max, 5000);

	return bwu_ret;

}

