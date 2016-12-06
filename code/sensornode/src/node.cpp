#include "node.hpp"

Node::Node(void){

}

Node::Node(int id){
	node_id = std::bitset<4> (id);
}


void Node::put_data_packet(packed_data packed_data_in){
	data_out.insert(data_out.begin(),packed_data_in);
}
/*
Node_GPS::Node_GPS(int id, Packer_GPS* packer_gps_inst){
	node_id = id;
	packer = packer_gps_inst;
}

*/