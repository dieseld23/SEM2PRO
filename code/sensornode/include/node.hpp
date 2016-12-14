/*****************************************************************************
* University of Southern Denmark
* 2. Semester Project (SEM2PRO)
*
* MODULENAME.: node
*
* PROJECT....: sensornode
*
* DESCRIPTION:  The node class is responsible for handling node operations such
*               as stop, start and reset time. It is also responsible of 
*               creating time messages each time there is new data.
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

#include "data_packet.hpp"

#include <thread>  
#include <iostream>
#include <vector>
#include <bitset>     
#include <mutex>
#include <unistd.h>


/*****************************    Defines    *******************************/
#define MILLI_SECOND 1000


/*****************************   Variables   *******************************/
class Protocol;

enum states { stopped, clear, time_packing, get_data, send_data };

class Node {
private:
  long int ms;
  std::mutex ms_mutex;
  bool running;
  states state;
  Protocol* protocol;
  std::bitset<4> node_id;
  std::thread loop_out_thread;
  std::thread loop_in_thread;
  std::thread counter_thread;
  
  std::vector<data_packet> data_out;
  std::mutex data_out_mutex;
  std::vector<int> event_buffer;
  std::mutex event_buffer_mutex;

  void print_vector_bool(std::vector<bool> vector);
  data_packet get_data_from_buffer(void);
  int data_in_buffer_test(void);
  void counter(void);
  data_packet construct_time_packet(long int ms);
  std::vector<bool> bitset32_to_vector(std::bitset<32> bitset);
  int get_from_event_buffer(void);
  int evnt_in_event_buffer_test(void);
  void set_ms(long int ms_in);
  long int get_ms(void);
  void buffer_clear(void);
public:
 Node(int id);
 void start(void);
 void loop_out(void);
 void loop_in(void);
 void put_data_packet(data_packet);
 void set_protocol(Protocol* protocol_in);
 void put_event(int);
};

/****************************** End Of Module *******************************/

