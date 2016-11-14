/*
 * btns_leds_functions.h
 *
 *  Created on: 9 Nov 2016
 *      Author: catalin
 */

#ifndef SRC_BTNS_LEDS_FUNCTIONS_H_
#define SRC_BTNS_LEDS_FUNCTIONS_H_



#define BTNS_DEVICE_ID 			XPAR_AXI_GPIO_BTNS_DEVICE_ID
#define LEDS_DEVICE_ID 			XPAR_AXI_GPIO_LEDS_DEVICE_ID
#define INTC_DEVICE_ID			XPAR_PS7_SCUGIC_0_DEVICE_ID
#define INTC_GPIO_INTERRUPT_ID	XPAR_FABRIC_AXI_GPIO_BTNS_IP2INTC_IRPT_INTR
#define BTN_INT 				XGPIO_IR_CH1_MASK
#define LED_CHANNEL				1


XGpio LEDInst, BTNInst;
XScuGic INTCInst;

/*=============================== Buttons Handler ===============================*/
void BTN_Intr_Handler(void *InstancePtr)
{
	int hex = 0;

	XGpio_InterruptDisable(&BTNInst, BTN_INT);

	// Ignore additional button presses
	if ((XGpio_InterruptGetStatus(&BTNInst) & BTN_INT) != BTN_INT) { return; }

	hex = XGpio_DiscreteRead(&BTNInst, 1);

	//XGpio_DiscreteWrite(&LEDInst, LED_CHANNEL, hex);

	SendFrameLEDS(CanInstPtr, hex);

    (void)XGpio_InterruptClear(&BTNInst, BTN_INT);
    XGpio_InterruptEnable(&BTNInst, BTN_INT);
}


/*=============================== Interrupt System Setup ===============================*/
int InterruptSystemSetup(XScuGic *XScuGicInstancePtr)
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

/*=============================== Intc Init Function ===============================*/
int IntcInitFunction(u16 DeviceId,  XGpio *GpioInstancePtr)
{
	XScuGic_Config *IntcConfig;
	int status;

	// Interrupt controller initialisation
	IntcConfig = XScuGic_LookupConfig(DeviceId);
	status = XScuGic_CfgInitialize(&INTCInst, IntcConfig, IntcConfig->CpuBaseAddress);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Call to interrupt setup
	status = InterruptSystemSetup(&INTCInst);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Connect GPIO interrupt to handler
	status = XScuGic_Connect(&INTCInst,
					  	  	 INTC_GPIO_INTERRUPT_ID,
					  	  	 (Xil_ExceptionHandler)BTN_Intr_Handler,
					  	  	 (void *)GpioInstancePtr);
	if(status != XST_SUCCESS) return XST_FAILURE;



	// Enable GPIO interrupts interrupt
	XGpio_InterruptEnable(GpioInstancePtr, 1);
	XGpio_InterruptGlobalEnable(GpioInstancePtr);

	// Enable GPIO and timer interrupts in the controller
	XScuGic_Enable(&INTCInst, INTC_GPIO_INTERRUPT_ID);


	return XST_SUCCESS;
}


/*=============================== Initialize Peripherals ===============================
 *
 *				INITIALIZE THE PERIPHERALS & SET DIRECTIONS OF GPIO
*/
int initPeripherals(){
	int status;

	// Initialise LEDs
	status = XGpio_Initialize(&LEDInst, LEDS_DEVICE_ID);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Initialise Push Buttons
	status = XGpio_Initialize(&BTNInst, BTNS_DEVICE_ID);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Set LEDs direction to outputs
	XGpio_SetDataDirection(&LEDInst, 1, 0x00);
	// Set all buttons direction to inputs
	XGpio_SetDataDirection(&BTNInst, 1, 0xFF);

	// Initialize interrupt controller
	status = IntcInitFunction(INTC_DEVICE_ID, &BTNInst);
	if(status != XST_SUCCESS) return XST_FAILURE;

}

#endif /* SRC_BTNS_LEDS_FUNCTIONS_H_ */
