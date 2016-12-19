#pragma once

#include <iostream>
#include <thread>

#include "frontend.hpp"

class ui
{
public:
	ui(frontend *frontend);
private:
	void running();
	void clr_scrn();
	
	std::thread ui_t;
	frontend *frontend_o;
};