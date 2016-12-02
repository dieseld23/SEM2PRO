#include "packer.hpp"

Packer_GPS::Packer_GPS(void){
	std::vector<bool> messagetype_1;		//LAT - "0001"
	messagetype_1.push_back(0);
	messagetype_1.push_back(0);
	messagetype_1.push_back(0);
	messagetype_1.push_back(1);

	messagetypes.push_back(messagetype_1);


	std::vector<bool> messagetype_2;		//Long - "0010"
	messagetype_2.push_back(0);
	messagetype_2.push_back(0);
	messagetype_2.push_back(1);
	messagetype_2.push_back(0);

	messagetypes.push_back(messagetype_2);
}


Packer_GPS::Packer_GPS(GPS* gps_in){
	gps_inst = gps_in;

	std::vector<bool> messagetype_1;		//LAT - "0001"
	messagetype_1.push_back(0);
	messagetype_1.push_back(0);
	messagetype_1.push_back(0);
	messagetype_1.push_back(1);

	messagetypes.push_back(messagetype_1);


	std::vector<bool> messagetype_2;		//Long - "0010"
	messagetype_2.push_back(0);
	messagetype_2.push_back(0);
	messagetype_2.push_back(1);
	messagetype_2.push_back(0);

	messagetypes.push_back(messagetype_2);
}



int Packer_GPS::get_gps_data(std::vector<packed_data>* packed_gps_data){
	packed_data packed_data_inst;
	for(int i = 0; i<messagetypes.size(); i++){
		packed_data_inst.messagetype = messagetypes[i];
		if(i==LAT_MESSAGE_TYPE){												// We know to pack LAT
			packed_data_inst.data = get_lat_message();
		}
		else if(i == LONG_MESSAGE_TYPE){										//Pack LONG
			packed_data_inst.data = get_long_message();
		}
		packed_gps_data->push_back(packed_data_inst);
	}
	return 1;
}

std::vector<bool> Packer_GPS::bitset30_to_vector(std::bitset<30> bitset){
	std::vector<bool> vector;
	for(int i = 0; i<30; i++){
		vector.insert(vector.begin(), bitset[i] );
	}
	return vector;
}

std::vector<bool> Packer_GPS::double_coordinate_to_bits(double coordinate_double){
	int coordinate_int = coordinate_double * DECIMALS;
	std::bitset<30> coordinat_bit (coordinate_int);

	std::vector<bool> coordinate_vector;
	coordinate_vector = bitset30_to_vector(coordinat_bit);
	return coordinate_vector;
}

std::vector<bool> Packer_GPS::get_lat_message(void){
	double lat_d;
	lat_d = gps_inst->get_lat();
	std::vector<bool> lat_info;
	lat_info = double_coordinate_to_bits(lat_d);
	lat_info.insert(lat_info.begin(), gps_inst->get_lat_dir() );

	return lat_info;
}

std::vector<bool> Packer_GPS::get_long_message(void){
	double long_d;
	long_d = gps_inst->get_long();
	std::vector<bool> long_info;
	long_info = double_coordinate_to_bits(long_d);
	long_info.insert(long_info.begin(), gps_inst->get_long_dir() );

	return long_info;
}

