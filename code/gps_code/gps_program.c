/*****************************************************************************
* University of Southern Denmark
* 2. Semester Project (SEM2PRO)
*
* MODULENAME.: gps_program.c
*
* PROJECT....: SEM2PRO
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 161110  MJ   	Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "gps_program.h"

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

// Should calculate checksum

int string_compare(char *string, char *compare_string, int n_characters){
	int i;
	int equal = 1;
	for(i=0; i<n_characters;i++){
		if(string[i]!=compare_string[i]){
			equal = 0;
			break;
		}
	}
	return equal;
}

int get_state(char *gps_data){
	enum state_type state = UNKNOWN;

	if(string_compare(gps_data, "$GPTXT",6)){
		state = GPTXT;
	}
	else if(string_compare(gps_data, "$GPRMC",6)){
		state = GPRMC;
	}
	else if(string_compare(gps_data, "$GPVTG",6)){
		state = GPVTG;
	}
	else if(string_compare(gps_data, "$GPGGA",6)){
		state = GPGGA;
	}
	else if(string_compare(gps_data, "$GPGSA",6)){
		state = GPGSA;
	}
	else if(string_compare(gps_data, "$GPGSV",6)){
		state = GPGSV;
	}
	else if(string_compare(gps_data, "$GPGLL",6)){
		state = GPGLL;
	}				
	else{
		state = UNKNOWN;
	}

	return state;
}


int string_until_comma(char *message, char *string, int *index){
	int t;
	int end_of_string = 0;

	for(t = 0; t < 100; t++){
		if(message[*index]=='\n' || message[*index]=='\0'){
			end_of_string = 1;
			break;
		}
		else if(message[*index]!=','){
			string[t] = message[*index];
			*index = *index + 1;	
		}
		else{
			*index = *index + 1;
			string[t] = '\0';
			break;
		}
	}
	return end_of_string;
}

void get_data_string(FILE *fp, char *data_string){
	char tmp;
	int t = 0;
	while(1){
		fscanf(fp,"%c",&tmp);
		if (tmp == '\n') {
			data_string[t]='\0';
			break;
		}
		else {
			data_string[t]=tmp;
			t++;
		}
	}
}

void copy_string(char *string, char *string_to_copy, int n_characters){
	int i;
	for(i = 0; i<n_characters; i++){
		string[i] = string_to_copy[i];
	}
	string[i]= '\0';
}


void decode_RMC_message(char *message, gps_data_s *gps_data){
	int index = 0;
	char data_string[32];
	int end_of_string = 0;
	int temp=0;
	int i = 0;

	while(!end_of_string){
		end_of_string = string_until_comma(message, data_string, &index);

		if(i==1){				// Time information
			copy_string(gps_data->time,data_string,CHAR_TIME_LENGTH);
		}
		else if(i == 2){		// Status informatin
			gps_data->status = data_string[0];
		}
		else if(i == 3){		// Latitude information
			copy_string(gps_data->latitude,data_string,CHAR_LATITUDE_LENGTH);
		}
		else if(i == 4){		// Latitude direction information
			gps_data->latitude_dir = data_string[0];
		}
		else if(i == 5){		// Longitude information
			copy_string(gps_data->longitude,data_string,CHAR_LONGITUDE_LENGTH);
		}
		else if(i == 6){		// Longitude direction information
			gps_data->longitude_dir = data_string[0];
		}
		else if(i == 9){		// Date information
			copy_string(gps_data->date,data_string,CHAR_DATE_LENGTH);
		}
		i++;
	}
}

void print_full_gps_data(gps_data_s *gps_data){

	printf("Date: %s\n", gps_data->date);
	printf("Time: %s\n", gps_data->time);
	printf("Status: %c\n", gps_data->status);
	printf("Latitude & direction : %s, %c\n", gps_data->latitude,gps_data->latitude_dir);
	printf("Longitude & direction: %s, %c\n\n\n", gps_data->longitude,gps_data->longitude_dir);

}

int main()
{
	FILE *fp;
	char buff[255];
	char tmp;
	fp = fopen("data/gps_test_data.txt", "r");
	int i;
	enum state_type state = UNKNOWN;
	gps_data_s gps_data;

	for(i = 0; i<200;i++){			//SHOULD BE WHILE
	//while(1){
		get_data_string(fp, buff);
		//printf(" %s \n", buff );

		state = get_state(buff);

		switch(state){
			case GPTXT:

			break;

			case GPRMC:
			decode_RMC_message(buff, &gps_data);
			print_full_gps_data(&gps_data);
			break;

			case GPVTG:
				//Something
			break;

			case GPGGA:
				//Something
			break;

			case GPGSA:
				//Something
			break;

			case GPGSV:
				//Something
			break;

			case GPGLL:
				//Something
			break;

			case UNKNOWN:
				//Something
			break;
		}
	}
	fclose(fp);

	return 0;
}




/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/






