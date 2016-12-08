#include "node.hpp"
#include "protocol.hpp"

Node::Node(void){
		//DELEETE
}

Node::Node(int id){
	std::bitset<4> bitset(id);
	std::vector<bool> vector;

	for(int i = 0; i<4; i++){
		vector.insert(vector.begin(), bitset[i] );
	}
	node_id = vector;
}


void Node::put_data_packet(packed_data packed_data_in){
	this->data_out_mutex.lock();								//Blocking
	data_out.insert(data_out.begin(),packed_data_in);			// Put in beginning
	this->data_out_mutex.unlock();
}



void Node::start(void){
	ms = 0;
	node_thread = std::thread(&Node::loop,this);  				//Start node thread
	counter_thread = std::thread(&Node::counter,this);			//Start counter thread
}

void Node::loop(void){
	long int ms_now = 0;
	std::vector<packed_data> packets_to_send;
	packed_data time_packet;
	packed_data temp_packet;
	int data = 0;
	while(1){
		// SHOULD BE MADE INTO STATE MACHINE! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		data = 0;																			// Clear
		packets_to_send.clear();															// Clear
		ms_now = ms;																		// Get timestamp
		packets_to_send.insert(packets_to_send.begin(),construct_time_packet(ms_now));		// Put in the first spot
		while(data_in_buffer_test() == 1){													// Get all data packets available
			data = 1;
			temp_packet = get_data_from_buffer();
			temp_packet.node_id = this->node_id;											// Add node id
			packets_to_send.insert(packets_to_send.begin(),temp_packet); 										// Add to ...					
		}
		if(data == 1){
			while(!packets_to_send.empty()){
				temp_packet = packets_to_send.back();										// Get packet
				packets_to_send	.pop_back();												// Delete read packet
				protocol->put_data_packet(temp_packet);										// Put packet in protocol
			}
		}
				// SHOULD BE MADE INTO STATE MACHINE! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
}

void Node::counter(void){
	while(1){
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		ms++;
	/*if(ms%1000 == 0){
		std::cout <<"ms: "<<ms<<std::endl;
	} */
	}
}


void Node::print_vector_bool(std::vector<bool> vector){
	for (auto i : vector) {
		std::cout << i;
	}
	std::cout << '\n';
}

packed_data Node::get_data_from_buffer(void){
	packed_data data_packet;
 	//std::cout <<"I am here in get data!!!\n";
	if(this->data_out_mutex.try_lock()){
		//std::cout <<"I am here in get data!!!\n";
		if(!this->data_out.empty()){
			//std::cout <<"I am here in get data\n";
			data_packet = this->data_out.back();
			//std::cout<<"----------- Incoming message -----------\n";
			//std::cout<<"Messageid: ";
			//print_vector_bool(data_packet->messagetype);
			//std::cout<<"\nData: ";
			//print_vector_bool(data_packet->data);
			//std::cout<<", data size: "<< data_packet->data.size() << "... ";
			//std::cout<<"\n\n\n\n";
			this->data_out.pop_back();
		}
		this->data_out_mutex.unlock();
	}
	return data_packet;
}

int Node::data_in_buffer_test(void){
	int data_in_buffer = 0;
	if(this->data_out_mutex.try_lock()){
		if(!this->data_out.empty()){
			data_in_buffer = 1;
		}
		this->data_out_mutex.unlock();
	}
	return data_in_buffer;
}

packed_data Node::construct_time_packet(long int ms){

	std::vector<bool> messagetype;		// time messagetype - "000000"
	for(int i = 0; i<6; i++){
		messagetype.push_back(0);	
	}
	std::bitset<32> time_data_bitset(ms);
	std::vector<bool> time_data = bitset32_to_vector(time_data_bitset);

	packed_data packed_data1;
	packed_data1.node_id = this->node_id;
	packed_data1.messagetype = messagetype;
	packed_data1.data = time_data;

	return packed_data1;
}

std::vector<bool> Node::bitset32_to_vector(std::bitset<32> bitset){
	std::vector<bool> vector;
	for(int i = 0; i<32; i++){
		vector.insert(vector.begin(), bitset[i] );
	}
	return vector;
}


void Node::set_protocol(Protocol* protocol_in){
	protocol = protocol_in;
}