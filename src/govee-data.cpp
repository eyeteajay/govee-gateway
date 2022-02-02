#include <sys/types.h>
#include <sstream>
#include <math.h>
#include "govee-data.h"

#include <iostream>

using namespace std;

void GoveeData::decodeData(const char* const data)
{
	// https://community.home-assistant.io/t/read-govee-temperature-humidity-ble-advertisements-with-esp32-and-esphome/230449/8
	// https://github.com/Thrilleratplay/GoveeWatcher
	// https://github.com/asednev/govee-bt-client/blob/master/src/decode.ts
	int	base = ((int)data[4] << 16) + ((int)data[5] << 8) + (int)data[6];

	temperatureC = base/10000.0f;
	temperatureF = temperatureC * 9 / 5 + 32;
	humidity = (base % 1000) / 10.0f;
	battery = (int)data[7];

	// https://en.wikipedia.org/wiki/Dew_point#Calculating_the_dew_point
	//const double	a = 6.1121;
	const double	b = 17.368, c = 238.88, d = 234.5;	// for 0celsius ≤ T ≤ 50celsius (error ≤ 0.05%)
	double		gamma, vaporpress;

	vaporpress = exp((b - temperatureC/d) * (temperatureC/(c + temperatureC))) * humidity/100.0;
	gamma = log(vaporpress);

	dewC = c * gamma / (b - gamma);
	dewF = dewC * 9 / 5 + 32;

	/*
	int temp = ((signed char)(data[4]) << 8) | int(data[3]);
	humidity = float((int(data[6]) << 8 | int(data[5])))/100;
	battery = int(data[7]);
	ma = int(data[8]);
	temperatureC = float(temp)/100;
	temperatureF = temperatureC*9/5+32;
	*/
}

void GoveeData::calcAverage(std::queue<GoveeData>* it)
{
	signed int rssi_temp=0;             	//!< separate rssi variable to calculate rssi average
	int sz =it->size();							//!< size of iterator
	// read all values in queue and calculate averages
	while (!it->empty())
	{
		temperatureF += it->front().temperatureF;
		temperatureC += it->front().temperatureC;
		dewF += it->front().dewF;
		dewC += it->front().dewC;
		humidity += it->front().humidity;
		battery += it->front().battery;
		rssi_temp += (it->front().rssi);
		ma += (it->front().ma);
		it->pop();
	}
	temperatureF = temperatureF / sz;
	temperatureC = temperatureC / sz;
	dewF /= sz;
	dewC /= sz;
	humidity = humidity / sz;
	battery = battery / sz;
	rssi_temp = int(rssi_temp / sz);
	ma = ma /sz ;
	rssi = static_cast<signed char>(rssi_temp);
}
