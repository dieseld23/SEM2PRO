
/*****************************************************************************
* University of Southern Denmark
* 2. Semester Project (SEM2PRO)
*
* MODULENAME.: node
*
* PROJECT....: sensornode
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 161127  MJ   	Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "node.hpp"
#include "protocol.hpp"



/*****************************   Functions   *******************************/

/*****************************************************************************
*   Input    : id
*   Output   : 
*   Function : Construct the node with node id
******************************************************************************/
Node::Node(int id){
	running = 1;
	state = stopped;
	std::bitset<4> bitset_id(id);
	node_id = bitset_id;
}

/*****************************************************************************
*   Input    : data_packet
*   Output   : 
*   Function : puts data packet in data_in buffer while handling mutex
******************************************************************************/
void Node::put_data_packet(data_packet packed_data_in){
	this->data_out_mutex.lock();								//Blocking
	data_out.insert(data_out.begin(),packed_data_in);			//Put in beginning
	this->data_out_mutex.unlock();
}

/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : starts threads for the node
******************************************************************************/
void Node::start(void){
	ms = 0;
	counter_thread = std::thread(&Node::counter,this);					//Start counter thread
	loop_out_thread = std::thread(&Node::loop_out,this);  				//Start node thread
	loop_in_thread = std::thread(&Node::loop_in,this);  				//Start node thread
}

/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function :  Loop for data coming from the CAN network. Gets data from the 
*				event buffer and acts accordingly.
******************************************************************************/
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

/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Loop for data going out of the node. State machine. 
*				If the node is in running mode, the function will get data from
*				data_out buffer and send the data after sending a time message.
******************************************************************************/
void Node::loop_out(void){
	long int ms_now = 0;
	std::vector<data_packet> packets_to_send;
	data_packet time_packet;
	data_packet temp_packet;
	int data = 0;
	while(1){
		switch(state) {
			case stopped: 
    					if(running == 1){
    						state = clear;
    					}
            			break;
            case clear: 
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
						while(!packets_to_send.empty()){
							temp_packet = packets_to_send.back();										// Get packet
							packets_to_send	.pop_back();												// Delete read packet
							protocol->put_data_packet(temp_packet);										// Put packet in protocol
						}
						state = clear;
						break;
		}
	}
}

/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : The function will run in its own thread and count ms up.
******************************************************************************/
void Node::counter(void){
	long int temp_ms;
	while(1){
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		temp_ms = get_ms();
		temp_ms++;
		set_ms(temp_ms);
	}
}

/*****************************************************************************
*   Input    : vector of bools
*   Output   : -
*   Function : prints a vector of bools
******************************************************************************/
void Node::print_vector_bool(std::vector<bool> vector){
	for (auto i : vector) {
		std::cout << i;
	}
	std::cout << '\n';
}

/*****************************************************************************
*   Input    : -
*   Output   : data_packet
*   Function : get data from data_out buffer while handling mutex
******************************************************************************/
data_packet Node::get_data_from_buffer(void){
	data_packet data_packet;
	if(this->data_out_mutex.try_lock()){
		if(!this->data_out.empty()){
			data_packet = this->data_out.back();
			this->data_out.pop_back();
		}
		this->data_out_mutex.unlock();
	}
	return data_packet;
}

/*****************************************************************************
*   Input    : -
*   Output   : if there is data in buffer
*   Function : tests if there is data in buffer
******************************************************************************/
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

/*****************************************************************************
*   Input    : ms
*   Output   : data_packet
*   Function : constructs a time packet with ms
******************************************************************************/
data_packet Node::construct_time_packet(long int ms){

	std::bitset<6> messagetype(0);								// messagetype 000000
	std::bitset<32> time_data_bitset(ms);
	std::vector<bool> time_data = bitset32_to_vector(time_data_bitset);

	data_packet packed_data1;
	packed_data1.node_id = this->node_id;
	packed_data1.messagetype = messagetype;
	packed_data1.data = time_data;

	return packed_data1;
}

/*****************************************************************************
*   Input    : bitset<32>
*   Output   : vector<bool>
*   Function : converts bitset<32> to vector
******************************************************************************/
std::vector<bool> Node::bitset32_to_vector(std::bitset<32> bitset){
	std::vector<bool> vector;
	for(int i = 0; i<32; i++){
		vector.insert(vector.begin(), bitset[i] );
	}
	return vector;
}

/*****************************************************************************
*   Input    : pointer to protocol
*   Output   : -
*   Function : sets protocol
******************************************************************************/
void Node::set_protocol(Protocol* protocol_in){
	protocol = protocol_in;
}

/*****************************************************************************
*   Input    : event
*   Output   : -
*   Function : Puts event in event_buffer while handling mutex
******************************************************************************/
void Node::put_event(int event){
	this->event_buffer_mutex.lock();							// Blocking
	event_buffer.insert(event_buffer.begin(),event);			// Put in beginning
	this->event_buffer_mutex.unlock();
}

/*****************************************************************************
*   Input    : -
*   Output   : event
*   Function : Gets event from event_buffer while handling mutex
******************************************************************************/
int Node::get_from_event_buffer(void){
	int event;
	if(this->event_buffer_mutex.try_lock()){
		if(!this->event_buffer.empty()){
			event = this->event_buffer.back();
			this->event_buffer.pop_back();
		}
		this->event_buffer_mutex.unlock();
	}
	return event;
}

/*****************************************************************************
*   Input    : -
*   Output   : if there is data in event buffer
*   Function : tests if there is data in event buffer
******************************************************************************/
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

/*****************************************************************************
*   Input    : ms
*   Output   : -
*   Function : sets ms
******************************************************************************/
void Node::set_ms(long int ms_in){
	this->ms_mutex.lock();								//Blocking
	this->ms = ms_in;
	this->ms_mutex.unlock();
}

/*****************************************************************************
*   Input    : -
*   Output   : ms
*   Function : gets ms
******************************************************************************/
long int Node::get_ms(void){
	long int ms_out;
	this->ms_mutex.lock();								//Blocking
	ms_out = this->ms;
	this->ms_mutex.unlock();

	return ms_out;
}