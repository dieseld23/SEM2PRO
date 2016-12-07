/*
 * simulation.c
 *
 *  Created on: 10 Nov 2016
 *      Author: Catalin I. Ntemkas
 */

#include "simulation.h"
#include "can_network.h"

/******************************************************************************
 * 							BUTTONS SIMULATION
 *****************************************************************************/
//============================================================== Handler Interrupts
void Handler_BtnsInterrupts(void *InstancePtr)
{
	int btnHex = 0;

	XGpio_InterruptDisable(&BTNInst, BTN_INT);

	// Ignore additional button presses
	if ((XGpio_InterruptGetStatus(&BTNInst) & BTN_INT) != BTN_INT) { return; }

	btnHex = XGpio_DiscreteRead(&BTNInst, 1);

	//XGpio_DiscreteWrite(&LEDInst, LED_CHANNEL, hex);
	SendFrame(CanInstPtr, btnHex);

    (void)XGpio_InterruptClear(&BTNInst, BTN_INT);
    XGpio_InterruptEnable(&BTNInst, BTN_INT);
}
//============================================================== Setup Functions
int setupInterruptSystem_Btns(XScuGic *XScuGicInstancePtr)
{
	// Enable interrupt
	XGpio_InterruptEnable(&BTNInst, BTN_INT);
	XGpio_InterruptGlobalEnable(&BTNInst);

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			 	 	 	 	 	 (Xil_ExceptionHandler)XScuGic_InterruptHandler,
			 	 	 	 	 	 XScuGicInstancePtr);
	Xil_ExceptionEnable();

	return XST_SUCCESS;

}
//============================================================== Initialise Interrupt Controllers
int init_IntrCtrlFunction_Btns(u16 DeviceId,  XGpio *GpioInstancePtr)
{
	XScuGic_Config *IntcConfig;
	int status;

	// Interrupt controller initialisation
	IntcConfig = XScuGic_LookupConfig(DeviceId);
	status = XScuGic_CfgInitialize(&INTCInst, IntcConfig, IntcConfig->CpuBaseAddress);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Call to interrupt setup
	status = setupInterruptSystem_Btns(&INTCInst);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Connect GPIO interrupt to handler
	status = XScuGic_Connect(&INTCInst,
					  	  	 INTC_GPIO_BTNS_INTERRUPT_ID,
					  	  	 (Xil_ExceptionHandler)Handler_BtnsInterrupts,
					  	  	 (void *)GpioInstancePtr);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Enable GPIO interrupts interrupt
	XGpio_InterruptEnable(GpioInstancePtr, 1);
	XGpio_InterruptGlobalEnable(GpioInstancePtr);

	// Enable GPIO and timer interrupts in the controller
	XScuGic_Enable(&INTCInst, INTC_GPIO_BTNS_INTERRUPT_ID);


	return XST_SUCCESS;
}
//============================================================== Initialise Buttons Simulation System
int init_SimulationSystem_Btns(){
	int status;

	// Initialise LEDs and Push Buttons
	status = XGpio_Initialize(&LEDInst, LEDS_DEVICE_ID);
	if(status != XST_SUCCESS) return XST_FAILURE;
	status = XGpio_Initialize(&BTNInst, BTNS_DEVICE_ID);
	if(status != XST_SUCCESS) return XST_FAILURE;

	/* Set direction:
	 * 		LEDS -> Output		BTNS -> Input	*/
	XGpio_SetDataDirection(&LEDInst, 1, 0x00);
	XGpio_SetDataDirection(&BTNInst, 1, 0xFF);

	// Initialise interrupt controller
	status = init_IntrCtrlFunction_Btns(INTC_DEVICE_ID, &BTNInst);
	if(status != XST_SUCCESS) return XST_FAILURE;

	return XST_SUCCESS;
}






/******************************************************************************
 * 							INPUT PORT SIMULATION
 *****************************************************************************/
//============================================================== Handler Interrupts
void Handler_IPortInterrupts(void *InstancePtr)
{
	int value = 0;

	XGpio_InterruptDisable(&IPortInst, BTN_INT);

	// Ignore additional button presses
	if ((XGpio_InterruptGetStatus(&IPortInst) & BTN_INT) != BTN_INT) { return; }

	value = XGpio_DiscreteRead(&IPortInst, 1);
	if( state_IPort == STATE_DISABLED && value == 1)
		state_IPort = STATE_ENABLED;
	else if( state_IPort == STATE_ENABLED && value == 0 )
		state_IPort = STATE_DISABLED;

	//XGpio_DiscreteWrite(&LEDInst, LED_CHANNEL, hex);
	SendFrame(CanInstPtr, value);

    (void)XGpio_InterruptClear(&IPortInst, BTN_INT);
    XGpio_InterruptEnable(&IPortInst, BTN_INT);
}
//============================================================== Setup Functions
int setupInterruptSystem_IPort(XScuGic *XScuGicInstancePtr)
{
	// Enable interrupt
	XGpio_InterruptEnable(&IPortInst, BTN_INT);
	XGpio_InterruptGlobalEnable(&IPortInst);

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			 	 	 	 	 	 (Xil_ExceptionHandler)XScuGic_InterruptHandler,
			 	 	 	 	 	 XScuGicInstancePtr);
	Xil_ExceptionEnable();

	return XST_SUCCESS;

}
//============================================================== Initialise Interrupt Controllers
int init_IntrCtrlFunction_IPort(u16 DeviceId,  XGpio *GpioInstancePtr)
{
	XScuGic_Config *IntcConfig;
	int status;

	// Interrupt controller initialisation
	IntcConfig = XScuGic_LookupConfig(DeviceId);
	status = XScuGic_CfgInitialize(&INTCInst, IntcConfig, IntcConfig->CpuBaseAddress);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Call to interrupt setup
	status = setupInterruptSystem_IPort(&INTCInst);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Connect GPIO interrupt to handler
	status = XScuGic_Connect(&INTCInst,
					  	  	 INTC_GPIO_IPORT_INTERRUPT_ID,
					  	  	 (Xil_ExceptionHandler)Handler_IPortInterrupts,
					  	  	 (void *)GpioInstancePtr);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Enable GPIO interrupts interrupt
	XGpio_InterruptEnable(GpioInstancePtr, 1);
	XGpio_InterruptGlobalEnable(GpioInstancePtr);

	// Enable GPIO and timer interrupts in the controller
	XScuGic_Enable(&INTCInst, INTC_GPIO_IPORT_INTERRUPT_ID);


	return XST_SUCCESS;
}
//============================================================== Initialise IPort Simulation System
int init_SimulationSystem_IPort(){
	int status;

	// Initialise LEDs and Push Buttons
	status = XGpio_Initialize(&LEDInst, LEDS_DEVICE_ID);
	if(status != XST_SUCCESS) return XST_FAILURE;
	status = XGpio_Initialize(&IPortInst, IPORT_DEVICE_ID);
	if(status != XST_SUCCESS) return XST_FAILURE;

	/* Set direction:
	 * 		LEDS -> Output		BTNS -> Input	*/
	XGpio_SetDataDirection(&LEDInst, 1, 0x00);
	XGpio_SetDataDirection(&IPortInst, 1, 0xFF);

	// Initialise interrupt controller
	status = init_IntrCtrlFunction_IPort(INTC_DEVICE_ID, &IPortInst);
	if(status != XST_SUCCESS) return XST_FAILURE;

	return XST_SUCCESS;
}

//============================================================== Initialise Simulation System
int init_SimulationSystem(){

	if( SIMULATION == SIM_BTNS)
		return init_SimulationSystem_Btns();
	else if( SIMULATION == SIM_IPORT)
		return init_SimulationSystem_IPort();
	return XST_FAILURE;
}
