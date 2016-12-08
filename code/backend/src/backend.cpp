#include "backend.hpp"
#include "fifo.hpp"

#include <vector>
#include <math.h>

backend::backend(fifo *fifo) : fifo_o(fifo)
{
}

coordinate_t backend::get_coordinate()
{
	std::string lat, lon;
	char lat_p, lon_p;
	std::vector<data_t> v;
	coordinate_t c;

	while(!fifo_o->get(GPS_ID, v));

	if (!v.empty())
	{
		lat_p = (v.back().data.front()-0x30 == LAT_NORTH) ? 'N' : 'S';
		v.back().data.erase(v.back().data.begin());
		lat = v.back().data.substr(0,31);
		v.back().data.erase(
				v.back().data.begin(),
				v.back().data.begin()+31);
		lon_p = v.back().data.front()-0x30 == LONG_EAST ? 'E' : 'W';
		v.back().data.erase(v.back().data.begin());
		lon = v.back().data;

		c.lat_polarity = lat_p;
		c.long_polarity = lon_p;

		c.latitude  = static_cast<float> (std::stoi(lat,nullptr,2)) * std::pow(10,-7);
		c.longitude = static_cast<float> (std::stoi(lon,nullptr,2)) * std::pow(10,-7);
	}

	return c;
}