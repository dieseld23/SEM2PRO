#include "node.hpp"
#include "protocol.hpp"

Node::Node(void){
		//DELEETE
}

Node::Node(int id){
	running = 1;
	state = stopped;
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
	counter_thread = std::thread(&Node::counter,this);					//Start counter thread
	loop_out_thread = std::thread(&Node::loop_out,this);  				//Start node thread
	loop_in_thread = std::thread(&Node::loop_in,this);  				//Start node thread
}

void Node::loop_in(void){
	int event;
	while(1){
		if(evnt_in_event_buffer_test() == 1){
			event = get_from_event_buffer();
			if(event == SYNC_COMMAND){
				set_ms(0);												//Reset ms
			}
			else if(event == START_COMMAND){
				running = 1;
			}
			else if(event == STOP_COMMAND){
				running = 0;
			}
		}
	}
}

void Node::loop_out(void){
	long int ms_now = 0;
	std::vector<packed_data> packets_to_send;
	packed_data time_packet;
	packed_data temp_packet;
	int data = 0;
	while(1){
		//topped, clear, time_packing, get_data, send_data }
		switch(state) {
			case stopped: 
    					//std::cout << "NODE IS STOPPED!!!\n\n"; // prints "1"
    					if(running == 1){
    						state = clear;
    					}
            			break;       // and exits the switch
            case clear: 
            			//std::cout << "Clear!\n";
             			data = 0;																			// Clear
						packets_to_send.clear();															// Clear
						if(running == 0){
							state = stopped;
						}
						else{
							state = time_packing;
						}
						break;
			case time_packing:
						//std::cout << "Time packet!\n";
						ms_now = get_ms();																	// Get timestamp
						packets_to_send.insert(packets_to_send.begin(),construct_time_packet(ms_now));		// Put in the first spot
						if(running == 0){
							state = stopped;
						}
						else{
							state = get_data;
						} 
						break;
			case get_data:
						//std::cout << "get data!\n";
						while(data_in_buffer_test() == 1){													// Get all data packets available
							data = 1;
							temp_packet = get_data_from_buffer();
							temp_packet.node_id = this->node_id;											// Add node id
							packets_to_send.insert(packets_to_send.begin(),temp_packet); 					// Add to ...					
						}
						if(running == 0){
							state = stopped;
						}
						else if(data == 1){
							state = send_data;
						} 
						else{
							state = clear;
						}
						break;
			case send_data:
						//std::cout << "send data!\n";
						while(!packets_to_send.empty()){
							temp_packet = packets_to_send.back();										// Get packet
							packets_to_send	.pop_back();												// Delete read packet
							protocol->put_data_packet(temp_packet);										// Put packet in protocol
						}
						state = clear;
						break;
		}
		// SHOULD BE MADE INTO STATE MACHINE! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         /*


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
		*/
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


void Node::put_event(int event){
	this->event_buffer_mutex.lock();								//Blocking
	event_buffer.insert(event_buffer.begin(),event);			// Put in beginning
	this->event_buffer_mutex.unlock();
}

int Node::get_from_event_buffer(void){
	int event;
 	//std::cout <<"I am here in get data!!!\n";
	if(this->event_buffer_mutex.try_lock()){
		//std::cout <<"I am here in get data!!!\n";
		if(!this->event_buffer.empty()){
			//std::cout <<"I am here in get data\n";
			event = this->event_buffer.back();
			//std::cout<<"----------- Incoming message -----------\n";
			//std::cout<<"Messageid: ";
			//print_vector_bool(data_packet->messagetype);
			//std::cout<<"\nData: ";
			//print_vector_bool(data_packet->data);
			//std::cout<<", data size: "<< data_packet->data.size() << "... ";
			//std::cout<<"\n\n\n\n";
			this->event_buffer.pop_back();
		}
		this->event_buffer_mutex.unlock();
	}
	return event;
}

int Node::evnt_in_event_buffer_test(void){
	int event_in_buffer = 0;
	if(this->event_buffer_mutex.try_lock()){
		if(!this->event_buffer.empty()){
			event_in_buffer = 1;
		}
		this->event_buffer_mutex.unlock();
	}
	return event_in_buffer;
}

void Node::set_ms(long int ms_in){
	this->ms_mutex.lock();								//Blocking
	this->ms = ms_in;
	this->ms_mutex.unlock();
}

long int Node::get_ms(void){
	long int ms_out;
	this->ms_mutex.lock();								//Blocking
	ms_out = this->ms;
	this->ms_mutex.unlock();

	return ms_out;
}