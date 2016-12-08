#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

#include "fifo.hpp"

struct buffer {
	std::vector<std::string> strings;
	std::mutex mutex;
};

class interpreter
{
public:
	interpreter(fifo *fifo);
	bool put(std::string msg);
	
private:
	//void recv(buffer *s_buff);
	void intprt(buffer *s_buff);


	buffer s_buff;
	//std::thread recv_t;
	std::thread intr_t;
	fifo *fifo_o;
};