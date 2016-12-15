
/*****************************************************************************
* University of Southern Denmark
* 2. Semester Project (SEM2PRO)
*
* MODULENAME.: canlink
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
#include "protocol.hpp"
#include "canlink.hpp"

/*****************************   Functions   *******************************/

/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Constructor. Starts threads with loop_out and loop_in
******************************************************************************/
Protocol::Protocol(void){
	loop_out_thread = std::thread(&Protocol::loop_out,this);  					//Start node thread
	loop_in_thread = std::thread(&Protocol::loop_in,this);  					//Start node thread
}

/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function :  Gets data from data_out buffer. Divide into more packetes if 
*				datalength is more than 8 bytes. Calculates and appends data-
*				length. Sends messages to can_lin class.
******************************************************************************/
void Protocol::loop_out(void){
	data_packet data_packet_inst, temp_packet;
	std::vector<data_packet> data_to_send;
	int nw_msg = NW_MSG_TRUE;
	while(1){
		if(data_in_buffer_out_test()){												// If there is data in buffer
			data_packet_inst = get_data_from_buffer_out();							// Fetch data
			if(data_packet_inst.data.size() > EIGHT_BYTES){							// If data is more than 8 bytes
				nw_msg = NW_MSG_TRUE;
				while(!data_packet_inst.data.empty()){								// while not empty
					temp_packet.data.clear();										// Clear temp_packet data
					for(int i = 0; i<64; i++){
						if(data_packet_inst.data.empty()){
							break;
						}
						temp_packet.data.push_back(data_packet_inst.data.front());	// Copy first item of data_packet to temp_packet
						data_packet_inst.data.erase(data_packet_inst.data.begin());	// Erase first item of data_packet
					}
					temp_packet.nw_msg = nw_msg;									// Copy sof
					temp_packet.node_id = data_packet_inst.node_id;					// Copy node id
					std::bitset<4> n_data_bytes_t ( temp_packet.data.size()/8 );	// Calculate size
					temp_packet.dlc = n_data_bytes_t;								// Insert size
					temp_packet.messagetype = data_packet_inst.messagetype;			// Copy messagetype
					nw_msg = NW_MSG_FALSE;											// set sof to FALSE
  					data_to_send.insert(data_to_send.begin(),temp_packet);			// Put packet in data_to_send
  				}
  			}else{
  				append_n_of_data_bytes(&data_packet_inst);
  				data_packet_inst.nw_msg = NW_MSG_TRUE;
  				data_to_send.insert(data_to_send.begin(),data_packet_inst);
  			}

																					// All data to send is in data_to_send
			temp_packet = data_to_send.back();										// Get item
			data_to_send.pop_back();												// Delete item
			can_link->send_to_can(temp_packet);										// Send to CAN
		}
	}
}

/*****************************************************************************
*   Input    : data
*   Output   : -
*   Function : puts data in data_out buffer while handling mutex
******************************************************************************/
void Protocol::put_data_packet(data_packet packed_data_in){
	this->data_out_mutex.lock();												//Blocking
	data_out.insert(data_out.begin(),packed_data_in);
	this->data_out_mutex.unlock();
}

/*****************************************************************************
*   Input    : -
*   Output   : data
*   Function : Gets data from data_out buffer while handling mutex
******************************************************************************/
data_packet Protocol::get_data_from_buffer_out(void){
	data_packet data_packet_inst;
	if(this->data_out_mutex.try_lock()){
		if(!this->data_out.empty()){
			data_packet_inst = this->data_out.back();
			this->data_out.pop_back();
		}
		this->data_out_mutex.unlock();
	}
	return data_packet_inst;
}

/*****************************************************************************
*   Input    : -
*   Output   : true or false, that there is data in buffer
*   Function : Determines if there is data in buffer, while handling mutex
******************************************************************************/
int Protocol::data_in_buffer_out_test(void){
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
*   Input    : pointer to data packed_data struct
*   Output   : -
*   Function : Calculates and appends datalength to struct
******************************************************************************/
void Protocol::append_n_of_data_bytes(data_packet* packed_data_in){
	int n_bytes = packed_data_in->data.size()/8;
	std::bitset<4> bitset(n_bytes);
	packed_data_in->dlc = bitset;
}

/*****************************************************************************
*   Input    : can_link
*   Output   : -
*   Function : Sets can_link
******************************************************************************/
void Protocol::set_can_link(CAN_link* can_link_in){
	can_link = can_link_in;
}

/*****************************************************************************
*   Input    : messsage_id
*   Output   : -
*   Function : Puts messageid into data_in buffer while handling mutex
******************************************************************************/
void Protocol::put_messageid_from_can(std::bitset<11> bitset_in){
	this->data_in_mutex.lock();													//Blocking
	data_in.insert(data_in.begin(), bitset_in);
	this->data_in_mutex.unlock();	
}

/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function :  Gets data from buffer_in, extracts command bits and puts 
*				in nodes event buffer
******************************************************************************/
void Protocol::loop_in(void){
	std::bitset<11> bitset_11;
	std::bitset<2> bitset_2;
	int command;
	while(1){
		if(data_in_buffer_in_test()){																		// If there is data in buffer
			bitset_11 = get_data_from_buffer_in();															// Get data
			bitset_2 = bitset11_to_bitset2(bitset_11);														// First 9 bits is deleted
			command = bitset_2.to_ulong();																	// command is exracted
			if(command == SYNC_COMMAND || command == START_COMMAND || command == STOP_COMMAND ){			// If command is valid
				node->put_event(command);																	// Put in nodes event buffer
			}
		}
	}
}

/*****************************************************************************
*   Input    : -
*   Output   : data
*   Function : Gets data from data_in buffer while handling mutex
******************************************************************************/
std::bitset<11> Protocol::get_data_from_buffer_in(void){
	std::bitset<11> bitset;
	if(this->data_in_mutex.try_lock()){
		if(!this->data_in.empty()){
			bitset = this->data_in.back();
			this->data_in.pop_back();
		}
		this->data_in_mutex.unlock();
	}
	return bitset;
}

/*****************************************************************************
*   Input    : -
*   Output   : true or false, that there is data in buffer
*   Function : Determines if there is data in buffer, while handling mutex
******************************************************************************/
int Protocol::data_in_buffer_in_test(void){
	int data_in_buffer = 0;
	if(this->data_in_mutex.try_lock()){
		if(!this->data_in.empty()){
			data_in_buffer = 1;
		}
		this->data_in_mutex.unlock();
	}
	return data_in_buffer;
}

/*****************************************************************************
*   Input    : bitset 11
*   Output   : bitset 2
*   Function : Ignores 9 first bits and returns last 2
******************************************************************************/
std::bitset<2> Protocol::bitset11_to_bitset2(std::bitset<11> bitset_11){
	int temp;
	for(int i = 2; i<11; i++){
		bitset_11.set(i,0);
	}
	temp = bitset_11.to_ulong();
	std::bitset<2> bitset_2(temp);

	return bitset_2;
}

/*****************************************************************************
*   Input    : node
*   Output   : -
*   Function : Sets node
******************************************************************************/
void Protocol::set_node(Node* node_in){
	node = node_in;
}