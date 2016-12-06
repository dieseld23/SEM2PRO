#include "gps.hpp"

GPS::GPS(int t){
	if(t == 13){		//DUMMY
		latitude =  	55.3673383;						//55degree 22.04030minutes
		lat_dir = 		LAT_NORTH;
		longitude =  	10.4318467;						//10degree 25.91080minutes
		long_dir = 		LONG_EAST;
	}
}

double GPS::get_lat(void){
	return latitude;
}

double GPS::get_long(void){
	return longitude;
}


bool GPS::get_long_dir(void){
	return long_dir;
}

bool GPS::get_lat_dir(void){
	return lat_dir;
}

void GPS::readfile(buffer *buffer_in){
	FILE *fp;
	fp = fopen("../data/gps_test_data.txt", "r");
	char tmp;
	int t = 0;
	char data_string_char[255];

	while(1){
		t = 0;
		while(1){
			fscanf(fp,"%c",&tmp);
			
			if (tmp == '\n') {
				//std::cout<<"newline!\n";
				data_string_char[t]='\0';
				break;
			}
			else {
				//std::cout<<"not newline!\n";
				data_string_char[t]=tmp;
				t++;
			}
		}
		usleep(100000);
		std::string str(data_string_char);
		if(!str.empty()){
			buffer_in->mutex.lock();															// Blocking
			buffer_in->strings.emplace(buffer_in->strings.begin(),str);			//Place str at beginning
			//std::cout<<"line put: "<<str<<std::endl;
			buffer_in->mutex.unlock();
		}
	}	
	fclose(fp);
}

std::string GPS::get_data_string(){
	std::string str;
	if(this->string_buffer.mutex.try_lock()){
		if(!this->string_buffer.strings.empty()){
			str = this->string_buffer.strings.back();
		//	std::cout<<"line read: "<<str<<std::endl;
			this->string_buffer.strings.pop_back();
		}
		this->string_buffer.mutex.unlock();
	}
	return str;
}

void GPS::start(void){
	std::string buff;
	enum state_type state = UNKNOWN;
	while(1){
		buff = get_data_string();
		//printf(" %s \n", buff );
		state = get_state(&buff);				

		switch(state){
			case GPTXT:
				//Something
			break;

			case GPRMC:
			decode_RMC_message(buff);

			print_full_gps_data();
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
}

void GPS::set_path(std::string set_path){
	path = set_path;
}


state_type GPS::get_state(std::string *gps_data){
	enum state_type state = UNKNOWN;

	std::string str = *gps_data;
	str.resize(6);										//Message ID is in first 6 characters. Ex. $GPGLL

	if(str == "$GPTXT"){
		state = GPTXT;
	}
	else if(str == "$GPRMC"){
		state = GPRMC;
	}
	else if(str == "$GPVTG"){
		state = GPVTG;
	}
	else if(str == "$GPGGA"){
		state = GPGGA;
	}
	else if(str == "$GPGSA"){
		state = GPGSA;
	}
	else if(str == "$GPGSV"){
		state = GPGSV;
	}
	else if(str == "$GPGLL"){
		state = GPGLL;
	}
	else{
		state = UNKNOWN;
	}
	return state;
}



int GPS::string_until_comma(std::string message, char *string, int *index){
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



void GPS::decode_RMC_message(std::string message){
	int index = 0;
	char data_string[32];
	int end_of_string = 0;
	int temp=0;
	int i = 0;

	while(!end_of_string){
		end_of_string = string_until_comma(message, data_string, &index);
		if(i==RMC_MES_TIME){				// Time information
			std::string string_message(data_string);
			time = std::stod (string_message,nullptr)*100;
		}
		else if(i == RMC_MES_STATUS){		// Status informatin
			if(data_string[0]=='V'){
				this->status = STATUS_VOID;
			}
			else if (data_string[0]=='A'){
				this->status = STATUS_ACTIVE;
			}
		}
		else if(i == RMC_MES_LAT){		// Latitude information
			std::string degree_s(data_string,0,LATITUDE_DEGR_LENGTH);
			int degree = std::stoi(degree_s);
			std::string minutes_s(data_string,LATITUDE_DEGR_LENGTH,LATITUDE_DEGR_LENGTH+MINUTES_LENGTH);
			double minutes = std::stod(minutes_s);
			this->latitude = degree + minutes_to_degress(minutes);
		}
		else if(i == RMC_MES_LAT_DIR){		// Latitude direction information
			if(data_string[0]=='N'){
				this->lat_dir = LAT_NORTH;
			}
			else if(data_string[0]=='S'){
				this->lat_dir = LAT_SOUTH;
			}
		}
		else if(i == RMC_MES_LONG){		// Longitude information
			std::string degree_s(data_string,0,LONGITUDE_DEGR_LENGTH);
			int degree = std::stoi(degree_s);
			std::string minutes_s(data_string,LONGITUDE_DEGR_LENGTH,LONGITUDE_DEGR_LENGTH+MINUTES_LENGTH);
			double minutes = std::stod(minutes_s);
			this->longitude = degree + minutes_to_degress(minutes);
		}
		else if(i == RMC_MES_LONG_DIR){		// Longitude direction information
			if(data_string[0]=='E'){
				this->long_dir = LONG_EAST;
			}
			else if(data_string[0]=='W'){
				this->long_dir = LONG_WEST;
			}
		}
		else if(i == RMC_MES_DATE){		// Date information
			std::string date_s(data_string);
			this->date = std::stoi(date_s);
		}
		i++;
	}
}


void GPS::print_full_gps_data(void){
	printf("Date: %d\n", this->date);
	printf("Time: %d\n", this->time);
	printf("Status: %d\n", this->status);
	printf("Latitude & direction : %f, %i\n", this->latitude,this->lat_dir);
	printf("Longitude & direction: %f, %i\n\n\n", this->longitude,this->long_dir);
}

double GPS::minutes_to_degress(double minutes){
	return minutes/60;
}


void GPS::start_datacollection_io(void){

}

void GPS::start_datacollection_file(void){
	std::cout<<"h";
	read_from_file_thread = std::thread(&GPS::readfile,this,&this->string_buffer);  //START THREAD
}

void GPS::send_data_to_node(void){
	packer->send_data_to_node();
}

void GPS::set_packer(Packer_GPS* packer_in){
	packer = packer_in;
}