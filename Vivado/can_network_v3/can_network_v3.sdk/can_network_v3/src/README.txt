
-------------------------------------------------------------------------------
						Southern Denmark University
					MSc. in Engineering in Electronics
								2nd semester

			Project 2 - Monitoring of Sensor Data on the SDU Go-kart
-------------------------------------------------------------------------------

Title:		CAN Network Application Software for the Zynq-7000 PS
Version: 	v0.3
Date:		19/12/2016

------------------------------------------------------------------- v0.1 - v0.3
@uthor:		Catalin I. Ntemkas
Email:		cante16@student.sdu.dk

Group members:	Thomas Søndergaard Christensen
				Mikkel Skarup Jaedicke
				Martin Brøchner Andersen
				Catalin Ionut Ntemkas
-------------------------------------------------------------------------------

1. DESCRIPTION
--------------
This software was developed for the purpose of utilizing the CAN controllers
present on the Processing System of the Zynq-7000 chip on the Zybo board as a
step towards implementing a complete system of nodes communicating through CAN.
The present version includes initialisation functions for the various devices
such as buttons, leds, ports and the can controllers. It also includes
functions for the interrupt controller, for CAN packets and for simulation 
purposes (e.g. pressing buttons to send a message).


2. CHANGELOG
------------
	- v0.1:		First draft of code built on XCanPs polled example from Xilinx
				documentation. Functionality of buttons interrupts and
				controlling the LEDs was added.
	- v0.2:		Software structured into separate files. Further improvements
				and refactoring were made to ensure proper software execution
				following the new file structure. Simulation mode for external
				GPIO port was also added, including all proper functions for
				initialising the device and the interrupt handler.
	- v0.3:		Publisher-subscriber architecture and a simple protocol for the
				message id were added. Further refactoring and improvements to
				were made to ensure proper use of the new functions.

3. STRUCTURE
------------
The software has been structured in separate files, as described in the list
below:

	- main_program.c: 	Contains the main loop of the program and the call to
						the initialisation functions.
	- include_files.h:	Includes certain xilinx libraries that are used by the
						code of the other files.
	- simulation.c:		It contains the functions related to the buttons, leds
						and the ports.
	- simulation.h:		The header file for the simulation.c file.					
    - can_network.c:	It contains the functions related to the CAN network.
    					Specifically, functions for initialisation, sending
    					and receiving frames, encoding and decoding the message
    					id of a	frame and for checking the subscriptions of the
    					node following the publisher-subscriber architecture.
   	- can_network.h:	The header file for the can_network.c file.
   	
   	
4. CONFIGURATION
----------------
SIMULATION:	
			There are two modes for simulation, which the program can only
			enter during startup. In order to change the simulation mode, the
			definition SIMULATION, located in simulation.h file needs to be
			changed to one of the available values, SIM_BTNS and SIM_IPORT.
			SIM_BTNS initialises and makes use of the buttons and LEDs, while
			the SIM_IPORT utilises a GPIO port for external interrupts.
			
CAN_NETWORK:	
			The nodes identifiers present on the go-kart network, can be set in
			the can_network.h file. The definition NODE_ID then may be assigned
			to one of those values before running the application, depending on
			what node (Zybo board) will be loaded to.
			
			
5. FUTURE DEVELOPMENT
---------------------
This software was developed as a prototype running on the PS system of the Zynq
chip to test the CAN network. If future work includes further development on
the PS system, then this application can be used as a basis to develop on,
modifying it and adding functionalities as required.
In case of migrating fully it on Linux, significant modification would be
required, in which case complete code rewrite is highly recommended, having
this application as a base example.
Another possibility is to utilise Asymmetric Multiprocessing (AMP) running
Linux on CPU0 and this application on CPU1, communicating through shared
memory. For AMP implementation details refer to the Xilinx Application Note
XAPP 1078 (v1.0), February 14, 2013 by John McDougall. The functions present in
the can_network.c file, sendFifoPacket() and RecvFifoPacket() are meant for the
latter case and have been left open for the necessary implementation of
communication between the two running systems.

			
x. COPYRIGHT & LICENCING
------------------------
Students are free to fully modify the application software for further
development to accommodate their needs of future projects under the Master
programme in Engineering in Electronics at the Southern Denmark University, 
providing to retain the information about the @uthors and group members that
worked and will work to bring this project to life.

