#include <string>
#include <string.h>

#include "interpreter.hpp"

interpreter::interpreter(fifo *fifo) : fifo_o(fifo)
{
	intr_t = std::thread(&interpreter::intprt, this, &this->s_buff);
}

bool interpreter::put(std::string msg)
{
	if (s_buff.mutex.try_lock())
	{
		s_buff.strings.push_back(msg);
		s_buff.mutex.unlock();
		return true;
	}
	return false;
}

void interpreter::intprt(buffer *s_buff)
{
	std::string msg, msgid, millis, data;

	while(1)
	{
		while(!s_buff->mutex.try_lock());
		if (!s_buff->strings.empty())
		{
			msg = s_buff->strings.back();
			s_buff->strings.pop_back();
			//std::cout << s_buff->strings.size() << std::endl;
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