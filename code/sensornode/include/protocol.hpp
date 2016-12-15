/*****************************************************************************
* University of Southern Denmark
* 2. Semester Project (SEM2PRO)
*
* MODULENAME.: protocol
*
* PROJECT....: sensornode
*
* DESCRIPTION: 	On data going from sensor to CAN network the protocol has the 
* 				responsibility to divide packets into packetes with less than
*				or equal to 8 bytes of data. It also need sto calculate the 
*				datalength in bytes and append it.
*
*				On data going to the node from the CAN network the protocol 
*				needs to decode what command is sent and deliver this event 
*				to the nodes event buffer.
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
#include <mutex>
#include <thread>  
#include <vector>

#include "node.hpp"

/*****************************    Defines    *******************************/
#define EIGHT_BYTES		64
#define NW_MSG_TRUE		1
#define NW_MSG_FALSE	0
#define SYNC_COMMAND	0
#define START_COMMAND	1
#define STOP_COMMAND	2

/*****************************   Variables   *******************************/

class CAN_link;
class Node;

class Protocol {
private:
	CAN_link* can_link;
	Node* node;
	std::mutex data_out_mutex;
	std::vector<data_packet> data_out;
	std::mutex data_in_mutex;
	std::vector<std::bitset<11>> data_in;
	std::thread loop_in_thread;
	std::thread loop_out_thread;
	data_packet get_data_from_buffer_out(void);
	int data_in_buffer_out_test(void);
	std::bitset<11> get_data_from_buffer_in(void);
	int data_in_buffer_in_test(void);
	std::bitset<2> bitset11_to_bitset2(std::bitset<11> bitset_in);
	void append_n_of_data_bytes(data_packet* packed_data_in);
public:
	Protocol(void);
	void put_data_packet(data_packet packed_data_in);
	void put_messageid_from_can(std::bitset<11>);
	void loop_out(void);
	void loop_in(void);
	void set_can_link(CAN_link* can_link_in);
	void set_node(Node* node_in);
};

/****************************** End Of Module *******************************/

