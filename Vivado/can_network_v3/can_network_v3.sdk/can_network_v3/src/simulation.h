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
//==================================================== Simulation Modes
#define SIM_BTNS						1
#define SIM_IPORT						0
#define SIMULATION						SIM_BTNS

#define STATE_ENABLED					1
#define STATE_DISABLED					0
#define LEDS_DEVICE_ID 					XPAR_AXI_GPIO_LEDS_DEVICE_ID
#define BTNS_DEVICE_ID 					XPAR_AXI_GPIO_BTNS_DEVICE_ID
#define IPORT_DEVICE_ID					XPAR_AXI_GPIO_IN_INTR_DEVICE_ID
#define INTC_DEVICE_ID					XPAR_PS7_SCUGIC_0_DEVICE_ID
#define INTC_GPIO_BTNS_INTERRUPT_ID		XPAR_FABRIC_AXI_GPIO_BTNS_IP2INTC_IRPT_INTR
#define INTC_GPIO_IPORT_INTERRUPT_ID	XPAR_FABRIC_AXI_GPIO_IN_INTR_IP2INTC_IRPT_INTR
#define BTN_INT 						XGPIO_IR_CH1_MASK
#define LED_CHANNEL						1

//============================================================== Variable Definitions
XGpio LEDInst, BTNInst, IPortInst;
XScuGic INTCInst;
static int state_IPort;

//============================================================== Function Prototypes
void Handler_IPortInterrupts(void *InstancePtr);
void Handler_BtnsInterrupts(void *InstancePtr);
int init_SimulationSystem();

#endif /* SRC_SIMULATION_H_ */
