#include "server.hpp"
#include "session.hpp"

#include <thread>

int main(int argc, char const *argv[])
{
	std::string msg;

	try
  	{
	    if (argc != 2)
	    {
			std::cerr << "Usage: async_tcp_echo_server <port>\n";
			return 1;
	    }

	    boost::asio::io_service io_service;
	    server s(io_service, std::atoi(argv[1]));
	    
	    std::thread t([&io_service](){ io_service.run(); });

	    while(std::getline(std::cin,msg))
	    {
	    	s.write(msg);
	    }

    	t.join();
  	}
  	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

  	return 0;
}