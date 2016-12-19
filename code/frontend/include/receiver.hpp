#pragma once

#include <iostream>
#include <thread>

#include "interpreter.hpp"

class receiver
{
public:
	receiver(interpreter *intprt);
private:
	void recv();

	interpreter *intprt_o;
	std::thread recv_t;
};