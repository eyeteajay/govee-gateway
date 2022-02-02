#include <map>
#include <queue>

#ifndef __GoveeData_H
#define __GoveeData_H

class  GoveeData {
public:
	float temperatureC;			//!< temperature in C
	float temperatureF;			//!< temperature in F
	float humidity;					//!< humidity in %
	float dewC, dewF;					// dew point
	int battery;						//!< battery level in %
	signed char rssi;				//!< rssi in dBm
	signed char ma;					//!< mysterious attribute

	void decodeData(const char* const data);						//!< decode data from info packet
	void calcAverage(std::queue<GoveeData>* it);													//!< calculate average from iterator
	// constructor
	GoveeData() : temperatureC(0), temperatureF(0), humidity(0), dewC(0), dewF(0), battery(0), rssi(0), ma(0){ };
};

#endif //__GoveeData_H
