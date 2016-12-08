#include "packer.hpp"
#include "gps.hpp"

Packer_GPS::Packer_GPS(void){
	std::vector<bool> messagetype_1;		//LAT & LONG messagetype - "000001"
	messagetype_1.push_back(0);
	messagetype_1.push_back(0);
	messagetype_1.push_back(0);
	messagetype_1.push_back(0);
	messagetype_1.push_back(0);
	messagetype_1.push_back(1);

	messagetypes.push_back(messagetype_1);
}

std::vector<bool> Packer_GPS::bitset31_to_vector(std::bitset<31> bitset){
	std::vector<bool> vector;
	for(int i = 0; i<31; i++){
		vector.insert(vector.begin(), bitset[i] );
	}
	return vector;
}

std::vector<bool> Packer_GPS::double_coordinate_to_bits(double coordinate_double){
	int coordinate_int = coordinate_double * DECIMALS;
	std::bitset<31> coordinat_bit (coordinate_int);

	std::vector<bool> coordinate_vector;
	coordinate_vector = bitset31_to_vector(coordinat_bit);
	return coordinate_vector;
}

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
	std::vector<bool> test;
	std::vector<bool> test1;

	//print_vector_bool(lat_long_mes);
	return lat_long_mes;
}


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

void Packer_GPS::set_node(Node* node_in){
	node = node_in;
}

void Packer_GPS::set_gps(GPS* gps_in){
	gps = gps_in;
}


void Packer_GPS::print_vector_bool(std::vector<bool> vector){
	std::cout << "vector: ";
	for (auto i : vector) {
		std::cout << i;
	}
	std::cout << '\n';
}

std::vector<bool> Packer_GPS::append_vector(std::vector<bool> vector, std::vector<bool> vector_to_append){
	for(int i = 0; i<vector_to_append.size(); i++){
		vector.push_back(vector_to_append[i]);
	}
	return vector;
}
