#include "Flow.hpp"


Flow::Flow(std::vector<uint8_t> new_flow){
	memcpy(&src_ip, &new_flow[0], 4);
	memcpy(&pointer, &new_flow[4], 1);

	memcpy(&gtw0_ip, &new_flow[5], 4);
	memcpy(&gtw0_rvalue, &new_flow[9], 8);

	memcpy(&gtw1_ip, &new_flow[17], 4);
	memcpy(&gtw1_rvalue, &new_flow[21], 8);

	memcpy(&gtw2_ip, &new_flow[29], 4);
	memcpy(&gtw2_rvalue, &new_flow[33], 8);

	memcpy(&gtw3_ip, &new_flow[41], 4);
	memcpy(&gtw3_rvalue, &new_flow[45], 8);

	memcpy(&gtw4_ip, &new_flow[53], 4);
	memcpy(&gtw4_rvalue, &new_flow[57], 8);

	memcpy(&gtw5_ip, &new_flow[65], 4);
	memcpy(&gtw5_rvalue, &new_flow[69], 8);

	memcpy(&dst_ip, &new_flow[77], 4);

	if(pointer > 5){
		log(logERROR) << "Flow pointer too big!!!";
		pointer = 0;
		return;
	}
}

Flow::Flow(const Flow& old_flow){
		src_ip = old_flow.src_ip;
		pointer = old_flow.pointer;

		gtw0_ip = old_flow.gtw0_ip;
		gtw0_rvalue = old_flow.gtw0_rvalue;

		gtw1_ip = old_flow.gtw1_ip;
		gtw1_rvalue = old_flow.gtw1_rvalue;

		gtw2_ip = old_flow.gtw2_ip;
		gtw2_rvalue = old_flow.gtw2_rvalue;

		gtw3_ip = old_flow.gtw3_ip;
		gtw3_rvalue = old_flow.gtw3_rvalue;

		gtw4_ip = old_flow.gtw4_ip;
		gtw4_rvalue = old_flow.gtw4_rvalue;

		gtw5_ip = old_flow.gtw5_ip;
		gtw5_rvalue = old_flow.gtw5_rvalue;

		dst_ip = old_flow.dst_ip;
}

std::vector<uint8_t> Flow::to_byte_vector(){
	std::vector<uint8_t> flow_out(81);
	
	memcpy(&flow_out[0], &src_ip, 4);
	memcpy(&flow_out[4], &pointer, 1);

	memcpy(&flow_out[5], &gtw0_ip, 4);
	memcpy(&flow_out[9], &gtw0_rvalue, 8);

	memcpy(&flow_out[17], &gtw1_ip, 4);
	memcpy(&flow_out[21], &gtw1_rvalue, 8);

	memcpy(&flow_out[29], &gtw2_ip, 4);
	memcpy(&flow_out[33], &gtw2_rvalue, 8);

	memcpy(&flow_out[41], &gtw3_ip, 4);
	memcpy(&flow_out[45], &gtw3_rvalue, 8);

	memcpy(&flow_out[53], &gtw4_ip, 4);
	memcpy(&flow_out[57], &gtw4_rvalue, 8);

	memcpy(&flow_out[65], &gtw5_ip, 4);
	memcpy(&flow_out[69], &gtw5_rvalue, 8);

	memcpy(&flow_out[77], &dst_ip, 4);

	return flow_out;

}

uint32_t Flow::get_gtw_ip_at(uint8_t pointer){
	switch(pointer){
		case 0:
			return gtw0_ip;
		case 1:
			return gtw1_ip;
		case 2:
			return gtw2_ip;
		case 3:
			return gtw3_ip;
		case 4:
			return gtw4_ip;
		case 5:
			return gtw5_ip;		
		default:
			return 0;
	}
}

void Flow::set_gtw_ip_at(uint8_t pointer, uint32_t new_value){
	switch(pointer){
		case 0:
			gtw0_ip = new_value;
			break;
		case 1:
			gtw1_ip = new_value;
			break;
		case 2:
			gtw2_ip = new_value;
			break;
		case 3:
			gtw3_ip = new_value;
			break;
		case 4:
			gtw4_ip = new_value;
			break;
		case 5:
			gtw5_ip = new_value;
			break;		
		default:
			return;
	}
}

uint64_t Flow::get_gtw_rvalue_at(uint8_t pointer){
		switch(pointer){
		case 0:
			return gtw0_rvalue;
		case 1:
			return gtw1_rvalue;
		case 2:
			return gtw2_rvalue;
		case 3:
			return gtw3_rvalue;
		case 4:
			return gtw4_rvalue;
		case 5:
			return gtw5_rvalue;
		default:
			return 0;
	}
}

void Flow::set_gtw_rvalue_at(uint8_t pointer, uint64_t new_value){
		switch(pointer){
		case 0:
			gtw0_rvalue = new_value;
			break;
		case 1:
			gtw1_rvalue = new_value;
			break;
		case 2:
			gtw2_rvalue = new_value;
			break;
		case 3:
			gtw3_rvalue = new_value;
			break;
		case 4:
			gtw4_rvalue = new_value;
			break;
		case 5:
			gtw5_rvalue = new_value;
			break;
		default:
			0;
	}
}

bool Flow::operator==(const Flow &other) const
{ 
		return src_ip == other.src_ip &&
		pointer == other.pointer &&

		gtw0_ip == other.gtw0_ip &&
		gtw0_rvalue == other.gtw0_rvalue &&

		gtw1_ip == other.gtw1_ip &&
		gtw1_rvalue == other.gtw1_rvalue &&

		gtw2_ip == other.gtw2_ip &&
		gtw2_rvalue == other.gtw2_rvalue &&

		gtw3_ip == other.gtw3_ip &&
		gtw3_rvalue == other.gtw3_rvalue &&

		gtw4_ip == other.gtw4_ip &&
		gtw4_rvalue == other.gtw4_rvalue &&

		gtw5_ip == other.gtw5_ip &&
		gtw5_rvalue == other.gtw5_rvalue &&

		dst_ip == other.dst_ip;
}

