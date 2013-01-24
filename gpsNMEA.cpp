/*
 * gpsNMEA.cpp
 *
 *  Created on: Jan 21, 2013
 *      Author: marcel
 */

#include "gpsNMEA.h"
#include "Tools.h"
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace std;
using boost::lexical_cast;
using boost::bad_lexical_cast;

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
	if (chksumStart + 2 >= nmeaDgm.size()) return false; // can checksum (2 places) be included in this dgm?

	// 3. Check check sum
	string chkSumDgm = nmeaDgm.substr(chksumStart + 1, 2);
	string msg = nmeaDgm.substr(start + 1, string::npos);
	msg = msg.substr(0, msg.find("*"));

	return (chkSumDgm.compare(calcChecksum(msg)) == 0);
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

bool GPSNMEA::parseNmeaMsg(const std::string& nmeaDgm)
{
	// check if it is a correct NMEA dgm
	if (!isNmeaMsg(nmeaDgm)) return false;

	// remove NMEA header ($GP) and checksum as both are ok
	size_t start = nmeaDgm.find("$GP");
	size_t stop = nmeaDgm.find('*', start);
	string dgm = nmeaDgm.substr(start + 3, stop - 3);

	// extract type
	vector<string> fields = Tools::split(dgm, ',');

	cout << "Original NMEA:      " << nmeaDgm << endl;
	cout << "Expanding NMEA dgm: " << dgm << endl;
	unsigned int i = 0;
	for (vector<string>::const_iterator it = fields.begin(); it != fields.end();
			++it) {
		cout << "  [" << i << "]: " << *it << endl;
		++i;
	}

	if (fields[0].compare("GGA") == 0) {
		parseGGA(fields);
	}
	else if (fields[0].compare("RMC") == 0) {
		praseRMC(fields);
	}
	else if (fields[0].compare("GLL") == 0) {
		parseGLL(fields);
	}
	else {
		cerr << "Unknown NMEA GPS datagram of type: " << fields[0] << endl;
	}

	return true;
}

void GPSNMEA::parseGGA(const std::vector<std::string>& nmea)
{
	if (!nmea[0].compare("GGA") == 0) return;

	// TODO implement GGA time handling
	updateTime(nmea[1]);

	// set position
	setLatitude(
			lexical_cast<double>(nmea[2])
					* (nmea[3].compare("S") == 0 ? (-1.) : 1.) / 100.);
	setLongitude(
			lexical_cast<double>(nmea[4])
					* (nmea[5].compare("W") == 0 ? (-1.) : 1.) / 100.);
	setAltitude(lexical_cast<double>(nmea[9]));
	if (nmea[10].compare("M") != 0)
		cerr << "(GGA) Unknown unit for altitude: " << nmea[1] << endl;

	// gps quality
	unsigned int quality = lexical_cast<unsigned int>(nmea[6]);
	switch (quality) {
		case 0:
			setGpsState(GPS::dataNotValid);
			break;
		case 1:
			setGpsState(GPS::autonomousMode);
			break;
		case 2:
			setGpsState(GPS::differentialGPS);
			break;
		default:
			cerr << "(GGA) Unknown GPS State " << quality << endl;
			break;
	}

	// sat count in use (not in view)
	try {
		setSatCount(lexical_cast<unsigned short>(nmea[7]));
	}
	catch (const bad_lexical_cast& e) {
		cerr << "Error while parsing field 7 " << e.what() << endl;
	}
}

void GPSNMEA::parseGLL(const std::vector<std::string>& nmea)
{
	if (!nmea[0].compare("GLL") == 0) return;

	// set position
	setLatitude(
			lexical_cast<double>(nmea[1])
					* (nmea[2].compare("S") == 0 ? (-1.) : 1.) / 100.);
	setLongitude(
			lexical_cast<double>(nmea[3])
					* (nmea[4].compare("W") == 0 ? (-1.) : 1.) / 100.);

	// check if it is a GLL packet with extra fields
	if (nmea.size() > 5) {
		updateTime(nmea[5]);
		if (nmea[6].compare("V") == 0) {
			// current gps data is invalid
			setGpsState(GPS::dataNotValid);
		}
		else if (nmea[6].compare("A") == 0) {
			// gps ok
			if (getGpsState() < GPS::autonomousMode)
				setGpsState(GPS::autonomousMode);
		}
		else {
			setGpsState(GPS::dataNotValid);
			cerr << "(GLL) Unknown GPS state: " << nmea[2] << endl;
		}
	}
}

void GPSNMEA::praseRMC(const std::vector<std::string>& nmea)
{
	if (!nmea[0].compare("RMC") == 0) return;
	cout << "Parsing RMC" << endl;

	updateTime(nmea[1]);
	updateDate(nmea[9]);

	// set position
	setLatitude(
			lexical_cast<double>(nmea[3])
					* (nmea[4].compare("S") == 0 ? (-1.) : 1.) / 100.);
	setLongitude(
			lexical_cast<double>(nmea[5])
					* (nmea[6].compare("W") == 0 ? (-1.) : 1.) / 100.);

	setGroundSpeed(lexical_cast<double>(nmea[7]) * .51444);
	setCourse(lexical_cast<double>(nmea[8]));

	if (nmea[2].compare("V") == 0) {
		// current gps data is invalid
		setGpsState(GPS::dataNotValid);
	}
	else if (nmea[2].compare("A") == 0) {
		// gps ok
		if (getGpsState() < GPS::autonomousMode)
			setGpsState(GPS::autonomousMode);
	}
	else {
		setGpsState(GPS::dataNotValid);
		cerr << "(RMC) Unknown GPS state: " << nmea[2] << endl;
	}
}

void GPSNMEA::updateTime(const std::string& s)
{
	tm orig(getTime());
	orig.tm_hour = lexical_cast<int>(s.substr(0, 2));
	orig.tm_min = lexical_cast<int>(s.substr(2, 2));
	orig.tm_sec = lexical_cast<int>(s.substr(4, 2));
	setTime(orig);
}

void GPSNMEA::updateDate(const std::string& s)
{
	tm orig(getTime());
	orig.tm_mday = lexical_cast<int>(s.substr(0, 2));
	orig.tm_mon = lexical_cast<int>(s.substr(2, 2)) - 1;
	orig.tm_year = lexical_cast<int>(s.substr(4, 2));
	if (orig.tm_year < 80)
		orig.tm_year += 100;
	setTime(orig);
}
