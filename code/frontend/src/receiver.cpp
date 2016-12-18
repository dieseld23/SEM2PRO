#include "receiver.hpp"

receiver::receiver(interpreter *intprt) : intprt_o(intprt)
{
	recv_t = std::thread(&receiver::recv, this);
}

void receiver::recv()
{
	std::string msg;
	while(1)
	{
		std::cin >> msg;
		if(!msg.empty())
		{	
			while(!intprt_o->put(msg));
		}
	}
}