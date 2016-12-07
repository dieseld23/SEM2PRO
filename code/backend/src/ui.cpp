#include "ui.hpp"
#include <ncurses.h>

ui::ui(backend *backend)
{
	backend_o = backend;
	ui_t = std::thread(&ui::running, this);
}

void ui::running()
{
	while(1)
	{
		
		coordinate_t c = backend_o->get_coordinate();
		clr_scrn();
		std::cout << c.lat_polarity << " " << c.latitude << " " << c.long_polarity << " " << c.longitude << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void ui::clr_scrn()
{
	std::cout << std::string(100,'\n');
}