/*
 * can_network.c
 *
 *  Created on: 10 Nov 2016
 *      Author: Catalin I. Ntemkas
 */

#include "can_network.h"
#include "simulation.h"


//============================================================== Init CAN Net
int init_CANNet(){

	int Status;
	CanInstPtr = &Can;
	XCanPs_Config *ConfigPtr;

	/* Initialize the Can device. */
	ConfigPtr = XCanPs_LookupConfig(CAN_DEVICE_ID);

	if (CanInstPtr == NULL) {
		return XST_FAILURE;
	}
	Status = XCanPs_CfgInitialize(CanInstPtr, ConfigPtr, ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* Run self-test on the device, which verifies basic sanity of the
	 * device and the driver. */
	Status = XCanPs_SelfTest(CanInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* Enter Configuration Mode so we can setup Baud Rate Prescaler
	 * Register (BRPR) and Bit Timing Register (BTR). */
	XCanPs_EnterMode(CanInstPtr, XCANPS_MODE_CONFIG);
	while(XCanPs_GetMode(CanInstPtr) != XCANPS_MODE_CONFIG);

	/*  Setup Baud Rate Prescaler Register (BRPR) and
	 * Bit Timing Register (BTR). */
	XCanPs_SetBaudRatePrescaler(CanInstPtr, TEST_BRPR_BAUD_PRESCALAR);
	XCanPs_SetBitTiming(CanInstPtr, TEST_BTR_SYNCJUMPWIDTH,
				TEST_BTR_SECOND_TIMESEGMENT,

				TEST_BTR_FIRST_TIMESEGMENT);

	/* Enter Loop Back or Normal Mode. */
	XCanPs_EnterMode(CanInstPtr, XCANPS_MODE);
	while(XCanPs_GetMode(CanInstPtr) != XCANPS_MODE);

	/* Send a test frame. */
	Status = SendFrame(CanInstPtr, 15);
	if (Status != XST_SUCCESS) {
		return Status;
	}

	Status = RecvFrame(CanInstPtr);

	return Status;
}

//============================================================== Send Frame
int SendFrame(XCanPs *InstancePtr, int data){
	u8 *FramePtr;
	int Status;

	/*
	 * Under full implementation, the message ID is provided from higher level
	 * (Linux) and it would not need to be created here.
	 * For testing purposes, it is created locally using createMsgID()
	 */
	TxFrame[0] = (u32)createMsgID(0x0, NODE_ID, 0xb);
	TxFrame[1] = (u32)XCanPs_CreateDlcValue((u32)FRAME_DATA_LENGTH);

	FramePtr = (u8 *)(&TxFrame[2]);
	FramePtr[0] = data;

	// For testing purposes
	xil_printf("NodeID: %d -> Sending: %d with msg id: %x\n", NODE_ID, FramePtr[0], TxFrame[0]);

	/* Wait until TX FIFO has room. */
	while (XCanPs_IsTxFifoFull(InstancePtr) == TRUE);

	/* Now send the frame.
	 *
	 * Another way to send a frame is keep calling XCanPs_Send() until it
	 * returns XST_SUCCESS. No check on if TX FIFO is full is needed anymore
	 * in that case.
	 */
	Status = XCanPs_Send(InstancePtr, TxFrame);

	return Status;
}

//============================================================== Receive Frame
int RecvFrame(XCanPs *InstancePtr)
{
	u8 *FramePtr;
	int Status;

	/*
	 * Wait until a frame is received.
	 */
	while (XCanPs_IsRxEmpty(InstancePtr) == TRUE);

	/* Receive a frame and verify its contents. */
	Status = XCanPs_Recv(InstancePtr, RxFrame);
	if (Status == XST_SUCCESS) {

		if ((RxFrame[1] & ~XCANPS_DLCR_TIMESTAMP_MASK) != TxFrame[1])
			return XST_LOOPBACK_ERROR;

		FramePtr = (u8 *)(&RxFrame[2]);

		/* In case the node is not subscribed to a particular message type
		 * it should just ignore the packet and NOT forward it to Linux */
		if( amISubscribed( (u16)RxFrame[0]) ){
			// For testing purposes
			xil_printf("NodeID: %d -> Received: %d with msg id: %x.\n", NODE_ID, FramePtr[0], RxFrame[0] );
			XGpio_DiscreteWrite(&LEDInst, LED_CHANNEL, FramePtr[0]);

			sendFifoPacket((u16)RxFrame[0], (u8)RxFrame[1], FramePtr);
		}
	}
	return Status;
}

/*============================================================== Fifo Packet
 * These functions are meant for the communication between this software and
 * Linux through xillybus Fifo.
 * Under full implementation of the project, these functions would be more
 * complex in order to be able to read and write to the xillybus FIFO
 * effectively.
 */
struct Packet recvFifoPacket(u16 msgid, u8 sizeOfData, u8* data){

	struct Packet packet;
	packet.prioritybit = getPriorityBit(msgid);
	packet.senderID = getSenderID(msgid);
	packet.msgtype = getMsgType(msgid);
	packet.sizeOfData = sizeOfData;
	packet.data = data;

	return packet;
}
int sendFifoPacket(u16 msgid, u8 sizeOfData, u8* data){
	/* To be implemented in case of an actual fifo between this software
	 * and Linux.
	 */
	return XST_SUCCESS;
}
/*============================================================= Decode Message ID
 */
u8 getPriorityBit(u16 msgid){
	return ( (msgid >> 0xa) & 0x1 );
}
u8 getSenderID(u16 msgid){
	return ( (msgid >> 0x6) & 0xf );
}
u8 getMsgType(u16 msgid){
	return ( msgid & 0x3f );
}

/*============================================================= Message ID & Subscriptions
 * Normally the message ID is received from Linux.
 * This function is for testing purposes.
 */
u16 createMsgID(u8 priorityBit, u8 senderID, u8 msgType){

	/* It creates the 11bit message ID
	 *
	 *		Priority bit	Sender ID	Message type
	 *			1 bit		 4 bits			6 bits
	 */

	int msgID = priorityBit<<SHIFT_PRIORITYBIT | senderID;
	msgID = msgID<<SHIFT_SENDERID | msgType;
	return msgID;
}

u8 amISubscribed(u16 msgid){
	int i = 0;

	for( i=0 ; i<SUBSCRIPTIONS ; i++){
	if( subscriptions[i] == msgid )
	  return SUBSCRIBED;
	}
	xil_printf("NodeID %d: Not subscribed to %d!!!\n", NODE_ID, msgid);
	return NOTSUBSCRIBED;
}
