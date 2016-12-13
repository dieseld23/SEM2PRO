/*****************************************************************************
* University of Southern Denmark
* 2. Semester Project (SEM2PRO)
*
* MODULENAME.: canlink
*
* PROJECT....: sensornode
*
* DESCRIPTION: 	The canlink class has the responsibility to transmit and 
*				receive to/from CAN. (physical CAN-link is not implemented)
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 161127  MJ    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/

#pragma once
#include "node.hpp"

#include <string>
#include <sstream>

/*****************************    Defines    *******************************/
/*****************************   Variables   *******************************/
class Protocol;

class CAN_link {
private:
	Protocol* protocol;
	std::mutex data_out_mutex;
	std::vector<data_packet> data_out;
	std::thread loop_out_thread;
	std::thread loop_in_thread;
	data_packet get_data_from_buffer(void);
	int data_in_buffer_test(void);
public:
	CAN_link(void);
	void loop_in(void);
	void loop_out(void);
	void send_to_can(data_packet packed_data_in);
	void set_protocol(Protocol* protocol_in);
};

/****************************** End Of Module *******************************/

