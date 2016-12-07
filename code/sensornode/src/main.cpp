#include <iostream>
#include <vector>
#include <thread>   
#include <mutex>
#include <unistd.h>			// SHould be removed

#include "node.hpp"
#include "packer.hpp"
#include "gps.hpp"

#include <bitset>         // std::bitset		FJERN


//NOTES:
// install on zybo libpthread-stubs0-dev 
// invest in thread priority

using namespace std;
int main()
{
	cout.precision(50); 

	Node gps_node(1);					// New GPS node

	Packer_GPS packer_gps;				// New GPS packer
	packer_gps.set_node(&gps_node);

	GPS gps_inst(13);					// New GPS sensor
	gps_inst.set_path("/dev/ttyACM1");
	gps_inst.set_packer(&packer_gps);

	packer_gps.set_gps(&gps_inst);
	gps_node.start();
	// Start system
	gps_inst.start_datacollection_file();
	gps_inst.start();

	//std::thread thread = std::thread(&gps_inst.start;  //START THREAD




/*
	std::vector<bool>::iterator it;
	for(it=long_info.begin() ; it < long_info.end(); it++) {
		std::cout<< *it;  // prints d.
	}
*//*
	std::string str_dec = "55.11239213123";
	double i_doub = std::stod (str_dec,nullptr);
	cout <<"string = "<<str_dec<<"\n";
	cout <<"i_dec  = "<<i_doub<<"\n";
	for(int n = 0; n< test.size(); n++){
		cout <<"N: "<< n<<"\n";
		cout << "Messagetype: ";
		for (std::vector<bool>::const_iterator i =  test[n].messagetype.begin(); i !=  test[n].messagetype.end(); ++i){
			std::cout << *i << ' ';
		}

		cout << "Data: ";
		for (std::vector<bool>::const_iterator i =  test[n].data.begin(); i !=  test[n].data.end(); ++i){
			std::cout << *i;
		}
		cout <<"\n";
	}*/
		return 0;
	}


	//0010000100000000010110000100011000000110001101111010100001010100
	//--------------------------------