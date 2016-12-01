/*
 * can_network.h
 *
 *  Created on: 10 Nov 2016
 *      Author: Catalin I. Ntemkas
 */

#ifndef SRC_CAN_NETWORK_H_
#define SRC_CAN_NETWORK_H_

//=============================================================== Include Files
#include "include_files.h"

//============================================================== Define Section
#define XCANPS_MODE						XCANPS_MODE_NORMAL	//MODES: _LOOPBACK, _NORMAL
#define CAN_DEVICE_ID					XPAR_XCANPS_0_DEVICE_ID
#define XCANPS_MAX_FRAME_SIZE_IN_WORDS 	(XCANPS_MAX_FRAME_SIZE / sizeof(u32))
#define FRAME_DATA_LENGTH 				8  /* Frame Data field length */

#define indexFR_PUID		0
#define indexFR_DATA		1

/******************************************************************************
 * Nodes messages ID in the CAN Stack of 4 Zybos, from bottom to top.
 * Keep the IDs in the range of [0,..., NODES_NUM] to be used properly
 * with the subscriptions mechanism implemented in amISubscribed()
 *****************************************************************************/
#define SUBSCRIBED		1
#define NOT_SUBSCRIBED	0
#define NODES_NUM		4
#define NODE_0			0	// Node 0 -> bottom
#define NODE_1			1	// Node 1 -> middle bottom
#define NODE_2			2	// Node 2 -> middle top
#define NODE_3			3	// Node 3 -> top

#define NODE_MSG_ID		2000
#define NODE_ID			NODE_0


/*
 * The Baud Rate Prescaler Register (BRPR) and Bit Timing Register (BTR)
 * are setup such that CAN baud rate equals 40Kbps, assuming that the
 * the CAN clock is 24MHz. The user needs to modify these values based on
 * the desired baud rate and the CAN clock frequency. For more information
 * see the CAN 2.0A, CAN 2.0B, ISO 11898-1 specifications.
 */
/*
 * Timing parameters to be set in the Bit Timing Register (BTR).
 * These values are for a 40 Kbps baudrate assuming the CAN input clock
 frequency
 * is 24 MHz.
 */
#define TEST_BTR_SYNCJUMPWIDTH		3
#define TEST_BTR_SECOND_TIMESEGMENT	2
#define TEST_BTR_FIRST_TIMESEGMENT	15
/*
 * The Baud rate Prescalar value in the Baud Rate Prescaler Register (BRPR)
 * needs to be set based on the input clock  frequency to the CAN core and
 * the desired CAN baud rate.
 * This value is for a 40 Kbps baudrate assuming the CAN input clock frequency
 * is 24 MHz.
 */
#define TEST_BRPR_BAUD_PRESCALAR	29
//======================================================== Variable Definitions
/*
 * Buffers to hold frames to send and receive. These are declared as global so
 * that they are not on the stack.
 * These buffers need to be 32-bit aligned
 */
u32 TxFrame[XCANPS_MAX_FRAME_SIZE_IN_WORDS];
u32 RxFrame[XCANPS_MAX_FRAME_SIZE_IN_WORDS];
XCanPs Can;
XCanPs *CanInstPtr;

//========================================================= Function Prototypes
int init_CANNet();
int SendFrame(XCanPs *InstancePtr, int data);
int RecvFrame(XCanPs *InstancePtr);
int amISubscribed(int nodeID, int publisherID);


#endif /* SRC_CAN_NETWORK_H_ */
