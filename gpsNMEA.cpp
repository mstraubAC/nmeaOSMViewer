/*
 * gpsNMEA.cpp
 *
 *  Created on: Jan 21, 2013
 *      Author: marcel
 */

#include "gpsNMEA.h"
#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>

using namespace std;

GPSNMEA::GPSNMEA()
{
	// TODO Auto-generated constructor stub

}

GPSNMEA::~GPSNMEA()
{
	// TODO Auto-generated destructor stub
}

bool GPSNMEA::isNmeaMsg(const std::string& nmeaDgm) const
{
	/* typical NMEA dg: $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
	 *                  ^^^															  ^^^
	 *                  $GP indicates start of NMEA GPS dgm							  * indicates checksum follows
	 *
	 * 3. check Checksum (hexadecimal)
	 */

	// 1. Search substring for identifier $GP
	size_t start = nmeaDgm.find("$GP");
	if (start == string::npos) return false;

	// 2. Search for *
	size_t chksumStart = nmeaDgm.find("*", start);
	if (chksumStart == string::npos) return false;
	if (chksumStart+2 >= nmeaDgm.size()) return false; // can checksum (2 places) be included in this dgm?

	// 3. Check check sum
	string chkSumDgm = nmeaDgm.substr(chksumStart+1, 2);
	string msg = nmeaDgm.substr(start+1, string::npos);
	msg = msg.substr(0, msg.find("*"));

	return (chkSumDgm.compare(calcChecksum(msg))== 0);
}

std::string GPSNMEA::calcChecksum(const std::string& nmeaDgm) const
{
	uint8_t chkSum = nmeaDgm[0];
	for (unsigned int i = 1; i < nmeaDgm.size(); ++i) {
		chkSum ^= nmeaDgm[i];
	}

	// convert checksum to hexadecimal string output in uppercase
	stringstream s;
	s << hex << int(chkSum);
	string x(s.str());
	transform(x.begin(), x.end(), x.begin(), ::toupper);
	return x;
}
