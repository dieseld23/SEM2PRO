#include "node.hpp"

Node::Node(void){

}


Node_GPS::Node_GPS(int id, Packer_GPS* packer_gps_inst){
	node_id = id;
	packer = packer_gps_inst;
}

void Node_GPS::get_gps_data(void){

}