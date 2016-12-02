#pragma once
#include <vector>
#include <iostream>
#include "node.hpp"
#include "gps.hpp"
#include <bitset>  

#define DECIMALS 			10000000
#define LAT_MESSAGE_TYPE 	0
#define LONG_MESSAGE_TYPE 	1


class Packer {
protected: 
	std::vector< std::vector<bool> > messagetypes;
public:
	//Packer(void);
	//Packer(std::vector<int>);
	//VIRTUAL CLASSES HERE?
};


// Derived class
class Packer_GPS: public Packer {
	GPS* gps_inst;
public:
	Packer_GPS(void);
	Packer_GPS(GPS*);
	std::vector<bool> bitset30_to_vector(std::bitset<30>);
	std::vector<bool> double_coordinate_to_bits(double coordinate);
	std::vector<bool> get_lat_message(void);
	std::vector<bool> get_long_message(void);
	int get_gps_data(std::vector<packed_data>*);
};
