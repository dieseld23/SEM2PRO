#pragma once
#include <vector>
#include <iostream>
#include "node.hpp"

#include <bitset>  

#define DECIMALS 				10000000
#define LAT_LONG_MESSAGE_TYPE 	0

struct packed_data;
class GPS;
class Node;

class Packer {
protected: 
	std::vector< std::bitset<6> > messagetypes;
public:
	Node* node;
	void set_node(Node* node_in);
};

class Packer_GPS: public Packer {
private:
	GPS* gps;
	std::vector<bool> bitset31_to_vector(std::bitset<31>);
	std::vector<bool> double_coordinate_to_bits(double coordinate);
	std::vector<bool> get_lat_long_message(void);
	void print_vector_bool(std::vector<bool> vector);
	std::vector<bool> append_vector(std::vector<bool> vector, std::vector<bool> vector_to_append);
public:
	Packer_GPS(void);
	void send_data_to_node(void);
	void set_gps(GPS* gps_in);
};
