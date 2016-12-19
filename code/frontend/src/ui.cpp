#include "ui.hpp"
#include <iomanip>

ui::ui(frontend *frontend)
{
	frontend_o = frontend;
	ui_t = std::thread(&ui::running, this);
}

void ui::running()
{
	while(1)
	{
		coordinate_t c = frontend_o->get_coordinate();
		//clr_scrn();
		std::cout << std::setprecision(9) << c.lat_polarity << " " << c.latitude << " " << c.long_polarity << " " << c.longitude << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void ui::clr_scrn()
{
	std::cout << std::string(100,'\n');
}