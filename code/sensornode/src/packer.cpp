
/*****************************************************************************
* University of Southern Denmark
* 2. Semester Project (SEM2PRO)
*
* MODULENAME.: packer
*
* PROJECT....: sensornode
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 161127  MJ   	Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "packer.hpp"
#include "gps.hpp"

/*****************************   Functions   *******************************/

/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Constructs the Packer_GPS with predefined messagetype
******************************************************************************/
Packer_GPS::Packer_GPS(void){
	std::bitset<6> messagetype_1(1);		//LAT & LONG messagetype - "000001"
	messagetypes.push_back(messagetype_1);
}

/*****************************************************************************
*   Input    : bitset<31>
*   Output   : vector<bool>
*   Function : converts bitset<31> to vector<bool>
******************************************************************************/
std::vector<bool> Packer_GPS::bitset31_to_vector(std::bitset<31> bitset){
	std::vector<bool> vector;
	for(int i = 0; i<31; i++){
		vector.insert(vector.begin(), bitset[i] );
	}
	return vector;
}

/*****************************************************************************
*   Input    : double coordinate
*   Output   : vector<bool>
*   Function : converts a double coordinate to vector<bool>
******************************************************************************/
std::vector<bool> Packer_GPS::double_coordinate_to_bits(double coordinate_double){
	int coordinate_int = coordinate_double * DECIMALS;
	std::bitset<31> coordinat_bit (coordinate_int);

	std::vector<bool> coordinate_vector;
	coordinate_vector = bitset31_to_vector(coordinat_bit);
	return coordinate_vector;
}

/*****************************************************************************
*   Input    : -
*   Output   : vector<bool>
*   Function : packs latitude and longitude data in message
******************************************************************************/
std::vector<bool> Packer_GPS::get_lat_long_message(void){
	double lat_d, long_d;
	lat_d = gps->get_lat();
	long_d = gps->get_long();

	std::vector<bool> lat_info, long_info, lat_long_mes;

	lat_info = double_coordinate_to_bits(lat_d);
	lat_info.insert(lat_info.begin(), gps->get_lat_dir() );

	long_info = double_coordinate_to_bits(long_d);
	long_info.insert(long_info.begin(), gps->get_long_dir() );

	lat_long_mes = append_vector(lat_info,long_info);					// Joing lat and long into one 8 byte message

	return lat_long_mes;
}

/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Sends all data to node
******************************************************************************/
void Packer_GPS::send_data_to_node(void){
	packed_data packed_data_inst;

	for(int i = 0; i<messagetypes.size(); i++){
		packed_data_inst.messagetype = messagetypes[i];
		if(i==LAT_LONG_MESSAGE_TYPE){										// We know to pack LAT & LONG
			packed_data_inst.data = get_lat_long_message();
		}
		node->put_data_packet(packed_data_inst);	
	}
}

/*****************************************************************************
*   Input    : pointer to node
*   Output   : -
*   Function : sets node
******************************************************************************/
void Packer::set_node(Node* node_in){
	node = node_in;
}

/*****************************************************************************
*   Input    : pointer to GPS
*   Output   : -
*   Function : sets gps
******************************************************************************/
void Packer_GPS::set_gps(GPS* gps_in){
	gps = gps_in;
}

/*****************************************************************************
*   Input    : vector<bool>
*   Output   : -
*   Function : prints vector<bool>
******************************************************************************/
void Packer_GPS::print_vector_bool(std::vector<bool> vector){
	std::cout << "vector: ";
	for (auto i : vector) {
		std::cout << i;
	}
	std::cout << '\n';
}

/*****************************************************************************
*   Input    : two vector<bool>
*   Output   : vector<bool>
*   Function : appends vector<bool> to vector<bool>
******************************************************************************/
std::vector<bool> Packer_GPS::append_vector(std::vector<bool> vector, std::vector<bool> vector_to_append){
	for(int i = 0; i<vector_to_append.size(); i++){
		vector.push_back(vector_to_append[i]);
	}
	return vector;
}
