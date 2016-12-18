#include "fifo.hpp"

fifo::fifo()
{

}

bool fifo::put(std::string msgid, data_t data)
{
	if(mutex.try_lock())
	{	
		if (buffer.find(msgid) == buffer.end())
		{
			data_l list;
			make_entry(data,list);

			buffer.emplace(msgid,list);
		}
		else
		{
			buffer[msgid].first.push_back(data);
			
			if(buffer[msgid].first.size() > 1023)
			{	
				buffer[msgid].first.erase(buffer[msgid].first.begin());
			}
		}
		sem_set(msgid);
		mutex.unlock();
		return true;
	}
	return false;
}

bool fifo::get(std::string msgid, std::vector<data_t> &v)
{
	if(mutex.try_lock())
	{
		if(buffer[msgid].second)
		{
			v = buffer[msgid].first;

			sem_clr(msgid);
			mutex.unlock();
			return true;
		}
		mutex.unlock();
	}
	return false;
}

void fifo::make_entry(data_t d, data_l &l)
{
	bool s = false;
	std::vector<data_t> v;
	v.push_back(d);

	l = std::make_pair(v,s);
}

void fifo::sem_set(std::string msgid)
{
	buffer[msgid].second = true;
}

void fifo::sem_clr(std::string msgid)
{
	buffer[msgid].second = false;
}