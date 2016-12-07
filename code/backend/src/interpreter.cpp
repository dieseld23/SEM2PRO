#include <string>
#include <string.h>

#include "interpreter.hpp"

interpreter::interpreter(fifo *fifo) : fifo_o(fifo)
{
	recv_t = std::thread(&interpreter::recv, this, &this->s_buff);
	intr_t = std::thread(&interpreter::interprete, this, &this->s_buff);
}

void interpreter::recv(buffer *s_buff)
{
	std::string msg;
	while(1)
	{
		std::cin >> msg;
		
		while(!s_buff->mutex.try_lock());
		s_buff->strings.push_back(msg);
		s_buff->mutex.unlock();
	}
}

void interpreter::interprete(buffer *s_buff)
{
	std::string msg, msgid, millis, data;

	while(1)
	{
		while(!s_buff->mutex.try_lock());
		if (!s_buff->strings.empty())
		{
			msg = s_buff->strings.back();
			s_buff->strings.pop_back();
		}
		s_buff->mutex.unlock();

		if(msg.size())
		{
			msgid  = msg.substr(0,11);
			msg.erase(0,11);
			millis = msg.substr(0,32);
			msg.erase(0,32);
			data   = msg;
			
			while(!fifo_o->put(msgid, data_t(millis,data)));
			msg.clear();
		}
		
	}
}