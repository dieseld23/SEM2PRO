/*****************************************************************************
* University of Southern Denmark
* 2. Semester Project (SEM2PRO)
*
* MODULENAME.: gps
*
* PROJECT....: sensornode
*
* DESCRIPTION: 	The GPS class will take data from the physical GPS sensor and
*				update its variables. As for now it is implemented to take data
*				from a file with recorded GPS data.
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

#include "packer.hpp"

#include <iostream>
#include <fstream> 
#include <string>
#include <string.h>
#include <mutex>
#include <vector>
#include <thread>  
#include <unistd.h>

/*****************************    Defines    *******************************/
#define LAT_NORTH	0
#define LAT_SOUTH	1
#define LONG_EAST	0
#define LONG_WEST	1

#define LATITUDE_DEGR_LENGTH 	2
#define LONGITUDE_DEGR_LENGTH 	3

#define MINUTES_LENGTH			8

#define STATUS_VOID				0
#define STATUS_ACTIVE			1

#define RMC_MES_TIME 		1
#define RMC_MES_STATUS		2
#define RMC_MES_LAT			3
#define RMC_MES_LAT_DIR		4
#define RMC_MES_LONG 		5
#define RMC_MES_LONG_DIR 	6
#define RMC_MES_DATE		9

#define READ_FROM_IO		0
#define READ_FROM_FILE		1


/*****************************   Variables   *******************************/
class Packer_GPS;

enum state_type {GPTXT, GPRMC, GPVTG, GPGGA, GPGSA, GPGSV, GPGLL, UNKNOWN};

struct buffer {
	std::vector<std::string> strings;
	std::mutex mutex;
};

class GPS {
private:
	double latitude;				//degree.decimals
	bool lat_dir;
	double longitude;				//degree.decimals
	bool long_dir;
	bool status;					
	int time;						//time 14:30:29:00 = 14302900
	int date;						//date 07/11/16 = 071116
	std::string path;
	buffer string_buffer;
	std::thread read_from_file_thread;
	std::thread loop_in_thread;

	Packer_GPS* packer;
	state_type get_state(std::string *gps_data);
	std::string get_data_string();
	int string_until_comma(std::string message, char *string, int *index);
	void decode_RMC_message(std::string message);
	void print_full_gps_data(void);
	double minutes_to_degress(double minutes);
	void send_data_to_node(void);
	void loop(void);
public:
	void start(void);
	void set_path(std::string);
	double get_lat(void);
	double get_long(void);	
	bool get_lat_dir(void);
	bool get_long_dir(void);
	void readfile(buffer* buffer_in);
	void start_datacollection_file(void);
	void start_datacollection_io(void);
	void set_packer(Packer_GPS*);
};

/****************************** End Of Module *******************************/

