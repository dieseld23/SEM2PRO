#pragma once

#include <iostream>
#include <thread>

#include "backend.hpp"

class ui
{
public:
	ui(backend *backend);
private:
	void running();
	void clr_scrn();
	
	std::thread ui_t;
	backend *backend_o;
};