/*
 * simulation.h
 *
 *  Created on: 10 Nov 2016
 *      Author: Catalin I. Ntemkas
 */

#ifndef SRC_SIMULATION_H_
#define SRC_SIMULATION_H_

//=============================================================== Include Files
#include "include_files.h"

//============================================================== Define Section
#define BTNS_DEVICE_ID 			XPAR_AXI_GPIO_BTNS_DEVICE_ID
#define LEDS_DEVICE_ID 			XPAR_AXI_GPIO_LEDS_DEVICE_ID
#define INTC_DEVICE_ID			XPAR_PS7_SCUGIC_0_DEVICE_ID
#define INTC_GPIO_INTERRUPT_ID	XPAR_FABRIC_AXI_GPIO_BTNS_IP2INTC_IRPT_INTR
#define BTN_INT 				XGPIO_IR_CH1_MASK
#define LED_CHANNEL				1

//======================================================== Variable Definitions
XGpio LEDInst, BTNInst;
XScuGic INTCInst;

//========================================================= Function Prototypes
void Handler_BtnInterrupts(void *InstancePtr);
int init_SimulationSystem();

#endif /* SRC_SIMULATION_H_ */
