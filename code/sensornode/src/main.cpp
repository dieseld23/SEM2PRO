#include <iostream>
#include <vector>
#include <thread>   
#include <mutex>
#include <unistd.h>			// SHould be removed

#include "node.hpp"
#include "packer.hpp"
#include "gps.hpp"

#include <bitset>         // std::bitset

// install on zybo libpthread-stubs0-dev 
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

	// Start system
	gps_inst.start_datacollection_file();
	gps_inst.start();





	std::vector<packed_data> test;
	
/*
	std::vector<bool>::iterator it;
	for(it=long_info.begin() ; it < long_info.end(); it++) {
		std::cout<< *it;  // prints d.
	}
*/
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
	}

	int k = 0;
	/*** THREAD ***/
	/*

	buffer buffer1;	
	
	std::thread first (foo,&buffer1);   
	while(1){
		if(buffer1.mutex.try_lock()){
			if(!buffer1.string_buffer.empty()){
				cout<<"hi";
				cout << buffer1.string_buffer.back()<<"\n";
				buffer1.string_buffer.pop_back();
			}	
			buffer1.mutex.unlock();
		}
	}
*/
	return 0;
}