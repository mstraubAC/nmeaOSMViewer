/*
 * gpsNMEA.h
 *
 * Parses NMEA messages according to
 * http://www.gpsinformation.org/dale/nmea.htm
 *
 * All positioning datums are required to be WGS84
 *
 *  Created on: Jan 21, 2013
 *      Author: marcel
 */

#ifndef GPSNMEA_H_
#define GPSNMEA_H_

#include "gps.h"
#include <string>

class GPSNMEA: public GPS
{
	public:
		GPSNMEA();
		GPSNMEA(const GPSNMEA& o);
		virtual ~GPSNMEA();
		GPSNMEA& operator=(const GPSNMEA& o);

		/**
		 * If the provided string is a complete NMEA message,
		 * the message is parsed and the internal state is
		 * changed accordingly
		 * @param nmeaDgm NMEA datagram
		 * @return true if the message was parsable, otherwise
		 * false is returned
		 */
		bool parseNmeaMsg(const std::string& nmeaDgm);

		/**
		 * Checks if the provided datagram is a valid NMEA
		 * datagram.
		 *
		 * @param nmeaDgm
		 * @return true if it is an NMEA message, otherwise false
		 */
		bool isNmeaMsg(const std::string& nmeaDgm) const;

	private:
		std::string calcChecksum(const std::string& nmeaDgm) const;

		// parser for different NMEA sentences
		void parseGGA(const std::string& nmeaDgm);
		void parseGLL(const std::string& nmeaDgm);
//		void parseGSA(const std::string& nmeaDgm);
		void praseRMC(const std::string& nmeaDgm);
};

#endif /* GPSNMEA_H_ */
