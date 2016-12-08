#include <iostream>
#include <vector>
#include <thread>   
#include <mutex>
#include <unistd.h>			// SHould be removed

#include "node.hpp"
#include "packer.hpp"
#include "gps.hpp"
#include "protocol.hpp"
#include "canlink.hpp"

//NOTES:
// install on zybo libpthread-stubs0-dev 
// invest in thread priority
// DATA IN BUFFER TEST AND SUCH FUNCTIONALITY SHOULD BE PUT IN BUFFER CLASS
// Destructors


using namespace std;
int main()
{
	cout.precision(50); 
	CAN_link can_link;					// New CAN_link
	Node gps_node(14);					// New GPS node
	Protocol protocol;					// New protocol
	protocol.set_can_link(&can_link);	// Set
	can_link.set_protocol(&protocol);

	Packer_GPS packer_gps;				// New GPS packer
	packer_gps.set_node(&gps_node);

	

	GPS gps_inst(13);					// New GPS sensor
	gps_inst.set_path("/dev/ttyACM1");
	gps_inst.set_packer(&packer_gps);

	packer_gps.set_gps(&gps_inst);		// Set
	gps_node.set_protocol(&protocol);	// Set
	gps_node.start();
	
	// Start system
	gps_inst.start_datacollection_file();
	gps_inst.start();



		return 0;
	}