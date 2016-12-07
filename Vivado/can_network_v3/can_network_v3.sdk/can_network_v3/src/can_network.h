/*
 * can_network.h
 *
 *  Created on: 10 Nov 2016
 *      Author: Catalin I. Ntemkas
 */

#ifndef SRC_CAN_NETWORK_H_
#define SRC_CAN_NETWORK_H_

//============================================================== Include Files
#include "include_files.h"

//============================================================== Define Section
#define XCANPS_MODE						XCANPS_MODE_NORMAL	//MODES: _LOOPBACK, _NORMAL
#define CAN_DEVICE_ID					XPAR_XCANPS_0_DEVICE_ID
#define XCANPS_MAX_FRAME_SIZE_IN_WORDS 	(XCANPS_MAX_FRAME_SIZE / sizeof(u32))
#define FRAME_DATA_LENGTH 				8  /* Frame Data field length */

//==================================================== Nodes IDs
#define NODES_NUM		4
#define NODE_ID_WIFI	0x1	// Bottom, 			0b0001
#define NODE_ID_IMU		0x3	// Middle bottom	0b0011
#define NODE_ID_SEV		0x7	// Middle top		0b0111
#define NODE_ID_GPS		0xE	// Top				0b1110

#define NODE_ID			NODE_ID_WIFI

//==================================================== Auxiliary definitions
#define SUBSCRIPTIONS	5
#define SUBSCRIBED		1
#define NOTSUBSCRIBED	0

#define SHIFT_PRIORITYBIT		0x4
#define SHIFT_SENDERID			0x4
#define SHIFT_MSGTYPE			0x2

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
#define TEST_BRPR_BAUD_PRESCALAR	3


//============================================================== Variable Definitions
/*
 * Buffers to hold frames to send and receive. These are declared as global so
 * that they are not on the stack.
 * These buffers need to be 32-bit aligned
 */
u32 TxFrame[XCANPS_MAX_FRAME_SIZE_IN_WORDS];
u32 RxFrame[XCANPS_MAX_FRAME_SIZE_IN_WORDS];
XCanPs Can;
XCanPs *CanInstPtr;

struct Packet {
	u8 prioritybit;
	u8 senderID;
	u8 msgtype;
	u8 sizeOfData;
	u8* data;
};
int subscriptions[SUBSCRIPTIONS];

//============================================================== Function Prototypes
int init_CANNet();
int SendFrame(XCanPs *InstancePtr, int data);
int RecvFrame(XCanPs *InstancePtr);
struct Packet recvFifoPacket( u16 msgid, u8 sizeOfData, u8* data);
int sendFifoPacket(u16 msgid, u8 sizeOfData, u8* data);
u8 getPriorityBit(u16 msgid);
u8 getSenderID(u16 msgid);
u8 getMsgType(u16 msgid);
u16 createMsgID(u8 priorityBit, u8 senderID, u8 msgType);
u8 amISubscribed(u16 msgid);


#endif /* SRC_CAN_NETWORK_H_ */
