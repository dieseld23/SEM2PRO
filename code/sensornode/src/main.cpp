#include <iostream>
#include <vector>
#include <thread>   
#include <mutex>
#include <unistd.h>

#include "node.hpp"
#include "packer.hpp"
#include "gps.hpp"
#include "protocol.hpp"
#include "canlink.hpp"

//NOTES:
// install on zybo libpthread-stubs0-dev 

int main()
{
	std::cout.precision(50); 
	CAN_link can_link;					// New CAN_link
	Node gps_node(14);					// New GPS node
	Protocol protocol;					// New protocol
	protocol.set_can_link(&can_link);	// Set
	protocol.set_node(&gps_node);

	can_link.set_protocol(&protocol);

	Packer_GPS packer_gps;				// New GPS packer
	packer_gps.set_node(&gps_node);

	
	GPS gps_inst;
	//GPS gps_inst(12);					// New GPS sensor
	gps_inst.set_path("/dev/ttyACM1");
	gps_inst.set_packer(&packer_gps);

	packer_gps.set_gps(&gps_inst);		// Set
	gps_node.set_protocol(&protocol);	// Set
	gps_node.start();

	// Start system
	gps_inst.start_datacollection_file();
	gps_inst.start();

	while(1){
		// Run forever
		usleep(1000);
	}
	return 0;
}