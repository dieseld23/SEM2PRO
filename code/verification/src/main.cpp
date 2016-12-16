#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>


int main(int argc, char const *argv[])
{
	std::string data_s, t_stamp, msgid, data_len;

	while(1)
	{
		std::cin >> t_stamp;
		std::cin >> data_s;

		t_stamp = t_stamp.substr(t_stamp.length()-32,t_stamp.length());
		msgid = data_s.substr(0,11);
		data_len = data_s.substr(11,15);

		std::cout << msgid << t_stamp  << data_s.substr(15,data_s.length()) << std::endl;
		usleep(500000);
	}
	
	return 0;
}