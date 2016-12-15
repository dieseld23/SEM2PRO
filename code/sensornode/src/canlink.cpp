
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
#include "canlink.hpp"
#include "protocol.hpp"



/*****************************   Functions   *******************************/

/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Constructor. Starts threads with loop_out and loop_in
******************************************************************************/
CAN_link::CAN_link(void){
	loop_out_thread = std::thread(&CAN_link::loop_out,this);  				//Start loop thread
	loop_in_thread = std::thread(&CAN_link::loop_in,this);  				//Start loop thread
}

/*****************************************************************************
*   Input    : data to send to can
*   Output   : -
*   Function : Sends data do can and handles mutex
******************************************************************************/
void CAN_link::send_to_can(data_packet packed_data_in){
	this->data_out_mutex.lock();								//Blocking
	data_out.insert(data_out.begin(),packed_data_in);
	this->data_out_mutex.unlock();
}

/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Loop that handles all data going out to CAN
******************************************************************************/
void CAN_link::loop_out(void){
	
	data_packet data_packet;
	while(1){
		if(data_in_buffer_test()){
			data_packet = get_data_from_buffer();
			data_packet.print();
			data_packet.print_bool_packet();
			std::cout << std::endl;
		}
	}
}

/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Loop that handles all data from CAN. 
******************************************************************************/
void CAN_link::loop_in(void){
	unsigned int n;
	while(1){
		std::string input;
		getline(std::cin, input);
		n = std::stod(input);
		std::bitset<11> bitset(n);
		protocol->put_messageid_from_can(bitset);
	}
}

/*****************************************************************************
*   Input    : -
*   Output   : data from buffer
*   Function : Gets one element from out-buffer while handling mutex
******************************************************************************/
data_packet CAN_link::get_data_from_buffer(void){
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
*   Output   : true or false, that there is data in buffer
*   Function : Determines if there is data in buffer, while handling mutex
******************************************************************************/
int CAN_link::data_in_buffer_test(void){
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
*   Input    : protocol
*   Output   : -
*   Function : Sets protocol
******************************************************************************/
void CAN_link::set_protocol(Protocol* protocol_in){
	protocol = protocol_in;
}
