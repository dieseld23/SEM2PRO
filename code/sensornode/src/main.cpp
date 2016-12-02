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
	GPS gps_inst(13);
	Packer_GPS packer_gps(&gps_inst);
	Node_GPS gps_node(1, &packer_gps);

	std::vector<packed_data> test;
	packer_gps.get_gps_data(&test);

	gps_inst.set_path("/dev/ttyACM1");
	gps_inst.start_datacollection_file();
	gps_inst.start();

	

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