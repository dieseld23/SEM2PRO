#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <mutex>
#include <utility>

struct data_t
{
	data_t(std::string millis, std::string data) : millis(millis), data(data){}
	std::string millis; //milliseconds since startup
	std::string data;
};

typedef std::pair< std::vector<data_t>, bool> data_l;

class fifo
{
public:
	fifo();
	bool put(std::string msgid, data_t data);
	bool get(std::string msgid, std::vector<data_t> &v);
private:
	void sem_set(std::string msgid);
	void sem_clr(std::string msgid);
	void make_entry(data_t d, data_l &l);

	std::mutex mutex;
	std::map<std::string, data_l > buffer;
};