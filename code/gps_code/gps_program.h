/*****************************************************************************
* University of Southern Denmark
* 2. Semester Project (SEM2PRO)
*
* MODULENAME.: gps_program.h
*
* PROJECT....: SEM2PRO
*
* DESCRIPTION: 	Program to read data from GPS sensor connected through serial USB.
				GPS data is a serial stream and the data is in the NMEA protocol.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 161110  MJ    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include<stdio.h>

/*****************************    Defines    *******************************/
#define CHAR_TIME_LENGTH 		9
#define CHAR_LATITUDE_LENGTH 	10
#define CHAR_LONGITUDE_LENGTH 	11
#define CHAR_DATE_LENGTH 		6

/*****************************   Variables   *******************************/
enum state_type {GPTXT, GPRMC, GPVTG, GPGGA, GPGSA, GPGSV, GPGLL, UNKNOWN};

typedef struct {
	char	time[10];
	char 	status;
	char	latitude[11];
	char 	latitude_dir;
	char	longitude[12];
	char	longitude_dir;
	char 	date[7];
} gps_data_s;

/*****************************   Functions   *******************************/

/*****************************************************************************
*   Input    : Pointer to gps_data string 
*   Output   : State
*   Function : Reads gps_data string, determines the message type and returns state
******************************************************************************/
int get_state(char *gps_data);


/*****************************************************************************
*   Input    : Pointer to string, Pointer to compare string, number of characters to compare
*   Output   : Equalily 
*   Function : Compares the first n_characters of the two strings. Returns 1 if equal.
******************************************************************************/
int string_compare(char *string, char *compare_string, int n_characters);


/*****************************************************************************
*   Input    : Pointer to message string, pointer to string, pointer to index
*   Output   : end_of_string (1 if the function reaches end of string)
*   Function : Puts the string from message[index] to next comma in string.
******************************************************************************/
int string_until_comma(char *message, char *string, int *index);


/*****************************************************************************
*   Input    : pointer to file id, pointer to data_string
*   Output   : -
*   Function : Reads from the file until newline (\n) and puts the string in data_string
******************************************************************************/
void get_data_string(FILE *fp, char *data_string);


/*****************************************************************************
*   Input    : pointer to string, pointer to string to copy, number of characters to copy
*   Output   : -
*   Function : Copies n_characters from string_to_copy to string.
******************************************************************************/
void copy_string(char *string, char *string_to_copy, int n_characters);


/*****************************************************************************
*   Input    : pointer to message string, pointer to gps_data_s struct
*   Output   : -
*   Function : Decodes an NMEA RMC message and puts approiate data in the gps_data_s struct
******************************************************************************/
void decode_RMC_message(char *message, gps_data_s *gps_data);


/*****************************************************************************
*   Input    : pointer to gps_data_s struct
*   Output   : -
*   Function : Prints all data from the gps_data_s struct by printf.
******************************************************************************/
void print_full_gps_data(gps_data_s *gps_data);


/****************************** End Of Module *******************************/

