/*
 * gps.cpp
 *
 *  Created on: Jan 21, 2013
 *      Author: marcel
 */

#include "gps.h"

GPS::GPS() : fGPSState(GPS::noFix), fSatCount(0), fTime(0), fLatitude(0), fLongitude(0), fAltitude(0), fHorizontalPrecision(10000)
{
}

GPS::GPS(const GPS& o)
{
	operator=(o);
}

GPS::~GPS()
{
}

GPS& GPS::operator=(const GPS& o)
{
	if (&o != this) {
		fGPSState = o.fGPSState;
		fSatCount = o.fSatCount;
		fTime = o.fTime;
		fLatitude = o.fLatitude;
		fLongitude = o.fLongitude;
		fAltitude = o.fAltitude;
		fHorizontalPrecision = o.fHorizontalPrecision;
	}
	return *this;
}

double GPS::getAltitude() const
{
	return fAltitude;
}

void GPS::setAltitude(double altitude)
{
	fAltitude = altitude;
}

GPS::GPSState GPS::getGpsState() const
{
	return fGPSState;
}

void GPS::setGpsState(GPS::GPSState gpsState)
{
	fGPSState = gpsState;
}

double GPS::getHorizontalPrecision() const
{
	return fHorizontalPrecision;
}

void GPS::setHorizontalPrecision(double horizontalPrecision)
{
	fHorizontalPrecision = horizontalPrecision;
}

double GPS::getLatitude() const
{
	return fLatitude;
}

void GPS::setLatitude(double latitude)
{
	fLatitude = latitude;
}

double GPS::getLongitude() const
{
	return fLongitude;
}

void GPS::setLongitude(double longitude)
{
	fLongitude = longitude;
}

uint8_t GPS::getSatCount() const
{
	return fSatCount;
}

void GPS::setSatCount(uint8_t satCount)
{
	fSatCount = satCount;
}

const tm& GPS::getTime() const
{
	return fTime;
}

void GPS::setTime(const tm& time)
{
	fTime = time;
}
