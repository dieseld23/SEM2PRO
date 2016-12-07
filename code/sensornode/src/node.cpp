#include "node.hpp"

Node::Node(void){

}

Node::Node(int id){
	node_id = std::bitset<4> (id);
}


void Node::put_data_packet(packed_data packed_data_in){
	this->data_out_mutex.lock();						//Blocking
	data_out.insert(data_out.begin(),packed_data_in);
	this->data_out_mutex.unlock();
}



void Node::start(void){
	std::cout <<"HEY\n";
	node_thread = std::thread(&Node::loop,this);  //START THREAD
}

void Node::loop(void){
	packed_data data_packet;
	while(1){
		if(get_data_from_buffer(&data_packet)){					//If there is data available
			
		}

	}
}

void Node::print_vector_bool(std::vector<bool> vector){
	for (auto i : vector) {
		std::cout << i;
	}
	std::cout << '\n';
}

int Node::get_data_from_buffer(packed_data* data_packet){
	int got_data = 0;
 	//std::cout <<"I am here in get data!!!\n";
	if(this->data_out_mutex.try_lock()){
		//std::cout <<"I am here in get data!!!\n";
		if(!this->data_out.empty()){
			//std::cout <<"I am here in get data\n";
			data_packet = &this->data_out.back();
			std::cout<<"----------- Incoming message -----------\n";
			std::cout<<"Messageid: ";
			print_vector_bool(data_packet->messagetype);
			std::cout<<"\nData: ";
			print_vector_bool(data_packet->data);
			std::cout<<"\n\n\n\n";
				//std::cout<<"line read: "<<str<<std::endl;
			this->data_out.pop_back();
			got_data = 1;
		}
		this->data_out_mutex.unlock();
	}
	return got_data;
}
