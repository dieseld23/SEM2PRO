#include "client.hpp"
#include <thread>

int main(int argc, char const *argv[])
{
	std::string msg;

    if (argc != 3)
    {
		std::cerr << "Usage: async_tcp_echo_server <host> <port>\n";
		return 1;
    }

    boost::asio::io_service io_service;
    client c(io_service, argv[1], argv[2]);

    std::thread t([&io_service](){ io_service.run(); });

    while(std::getline(std::cin,msg))
    {
    	c.write(msg);
    }

    t.join();

  	return 0;
}

