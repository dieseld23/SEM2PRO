/*****************************************************************************
* University of Southern Denmark
* 2. Semester Project (SEM2PRO)
*
* MODULENAME.: 
*
* PROJECT....: sensornode
*
* DESCRIPTION:  Simple struct to hold packed data
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
#include <iostream>
#include <vector>
#include <bitset>  
#include <sstream> 

struct data_packet {
  std::bitset<1> nw_msg;
  std::bitset<4> node_id;
  std::bitset<4> dlc;               //Number of data bytes
  std::bitset<6> messagetype;
  std::vector<bool> data;

/*****************************   Functions   *******************************/

/*****************************************************************************
*   Input    : 
*   Output   : 
*   Function : Prints the packet in bools
******************************************************************************/
void print_bool_packet(void){
  std::cout << nw_msg;
  std::cout <<node_id;
  std::cout<<messagetype;
  std::cout<<dlc;
  print_vector_bool(this->data);
}

/*****************************************************************************
*   Input    : 
*   Output   : 
*   Function : Prints the packet in bools
******************************************************************************/
void print_bool_packet_string(void){
 std::stringstream ss;

 ss << nw_msg.to_string()<<node_id.to_string()<<messagetype.to_string()<<dlc.to_string()<<test(this->data);
 std::cout <<ss.str();
//<< print_vector_bool(this->data);
}

/*****************************************************************************
*   Input    : vector<bool>
*   Output   : 
*   Function : Prints a vector of bools
******************************************************************************/
void print_vector_bool(std::vector<bool> vector){
  for (auto i : vector) {
    std::cout << i;
  }
}

 std::string  test(std::vector<bool> vector){
   std::string ss = "";
  for (auto i : vector) {
    if (i == true){
      ss += '1';
    }else{
      ss += '0'; 
    }
  }
  return ss;
}



/*****************************************************************************
*   Input    : 
*   Output   : 
*   Function : new message
******************************************************************************/
void print_sof(void){
  std::cout<< "Start of frame: ";
  std::cout << nw_msg << '\n';
}


/*****************************************************************************
*   Input    : 
*   Output   : 
*   Function : prints node id
******************************************************************************/
void print_node_id(void){
  std::cout<< "Node id: ";
  std::cout<< node_id;
  std::cout << '\n';
}


/*****************************************************************************
*   Input    : 
*   Output   : 
*   Function : prints number of bytes
******************************************************************************/
void print_n_data_bytes(void){
  std::cout<< "Number of data bytes: ";
  std::cout<<dlc << '\n';
}


/*****************************************************************************
*   Input    : 
*   Output   : 
*   Function : prints messagetype
******************************************************************************/
void print_messagetype(void){
  std::cout<< "Messagetype: ";
  std::cout<<messagetype << '\n';
  std::cout << '\n';
}


/*****************************************************************************
*   Input    : 
*   Output   : 
*   Function : prints data
******************************************************************************/
void print_data(void){
  std::cout<< "Data: ";
  print_vector_bool(this->data);
  std::cout << '\n';
}


/*****************************************************************************
*   Input    : 
*   Output   : 
*   Function : prints full data packet
******************************************************************************/
void print(void){
  print_sof();
  print_node_id();
  print_n_data_bytes();
  print_messagetype();
  print_data();
}
};

