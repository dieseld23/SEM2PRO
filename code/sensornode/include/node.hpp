#pragma once

#include <iostream>
#include <vector>
#include <bitset>      

/*** Forward Declarations ***/
class Packer_GPS;
/****************************/

struct packed_data {
    std::vector<bool> messagetype;
   	std::vector<bool> data;
};



class Node {
protected:
    std::bitset<4> node_id;
    std::vector<packed_data> data;
  public:
  	Node(void);

};

// Derived class
class Node_GPS: public Node {
	Packer_GPS* packer;
public:
	Node_GPS(int, Packer_GPS*);
	void get_gps_data(void);
};
