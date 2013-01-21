/*
 * gps.h
 *
 * Define structures and interfaces for position
 * information
 *
 *  Created on: Jan 21, 2013
 *      Author: marcel
 */

#ifndef GPS_H_
#define GPS_H_

#include <cinttypes>
#include <ctime>
#include <stdint.h>

//
class GPS {
	public:
		enum GPSState { noFix, fix, dgpsFix };

		GPS();
		GPS(const GPS& o);
		virtual ~GPS();

		GPS& operator=(const GPS& o);


		double getAltitude() const;
		void setAltitude(double altitude);
		GPSState getGpsState() const;
		void setGpsState(GPSState gpsState);
		double getHorizontalPrecision() const;
		void setHorizontalPrecision(double horizontalPrecision);
		double getLatitude() const;
		void setLatitude(double latitude);
		double getLongitude() const;
		void setLongitude(double longitude);
		uint8_t getSatCount() const;
		void setSatCount(uint8_t satCount);
		const tm& getTime() const;
		void setTime(const tm& time);

	private:
		GPSState fGPSState;
		uint8_t fSatCount;
		tm fTime; // UTC
		double fLatitude; // >0 = N / <0 = S
		double fLongitude; // >0 = E / <0 = W
		double fAltitude; // in m
		double fHorizontalPrecision; // in m
};
#endif /* GPS_H_ */
