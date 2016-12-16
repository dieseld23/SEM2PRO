#pragma once

#include <iostream>
#include <mutex>

#define GPS_ID "11110001001"
#define LAT_NORTH	0
#define LAT_SOUTH	1
#define LONG_EAST	0
#define LONG_WEST	1

class fifo;

struct coordinate_t
{
	char lat_polarity;
	float latitude;
	char long_polarity;
	float longitude;
};

class backend
{
public:
	backend(fifo *fifo);

	/*GPS Getters*/
	coordinate_t get_coordinate();
private:
	fifo *fifo_o;

};

