/******************************************************************************
 *						Southern Denmark University
 *
 *				Project 2 - MSc. in Engineering in Electronics
 *				A SCADA system to monitor data from the go-kart
 *
 *		Group members:
 *
 *
 * main_program.c
 *  Created on: 10 Nov 2016
 *      Author: Catalin I. Ntemkas
 *
 *****************************************************************************/

//=============================================================== Include Files
#include "include_files.h"
#include "simulation.h"
#include "can_network.h"

//===============================================================

//===============================================================
//===============================================================

int main(){

	if( init_SimulationSystem() == XST_FAILURE){
		xil_printf("FAILED: Initialising Simulation System...\n");
		return XST_FAILURE;
	}
	if( init_CANNet() ==  XST_FAILURE){
		xil_printf("FAILED: Initialising CAN Network...\n");
		return XST_FAILURE;
	}

	state_IPort = STATE_DISABLED;
	//================== Main Loop
	while(1){
		RecvFrame(CanInstPtr);
	}

	return XST_SUCCESS;
}

