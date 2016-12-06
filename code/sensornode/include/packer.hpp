#pragma once
#include <vector>
#include <iostream>
#include "node.hpp"
#include <bitset>  

#define DECIMALS 				10000000
#define LAT_LONG_MESSAGE_TYPE 	0

/** Forward declartion **/
struct packed_data;
class GPS;
class Node;

class Packer {
protected: 
	std::vector< std::vector<bool> > messagetypes;
public:
	//Packer(void);
	//Packer(std::vector<int>);
	//VIRTUAL CLASSES HERE?
	//OVERVEJ HVILKE FUNKTIONER OG VARIABLER DER KAN FLYTTES HEROP!!!
};


// Derived class
class Packer_GPS: public Packer {
private:
	Node* node;
	GPS* gps;
	std::vector<bool> bitset30_to_vector(std::bitset<30>);
	std::vector<bool> double_coordinate_to_bits(double coordinate);
	std::vector<bool> get_lat_long_message(void);
	std::vector<bool> get_long_message(void);
public:
	Packer_GPS(void);
	int get_gps_data(std::vector<packed_data>*);
	void send_data_to_node(void);
	void set_node(Node* node_in);
	void set_gps(GPS* gps_in);
};
