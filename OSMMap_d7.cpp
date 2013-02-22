/*
 * OSMMap.h
 *
 *  Created on: Jan 31, 2013
 *      Author: marcel
 */
//#include "OSMMap_d7.h"
#include <string>

#include "OSMMap.h"
#include "macros.h"

typedef OSMMap* OSMMapHandle;
extern "C"
{
	OSMMapHandle NewOSMMap(void) { return new OSMMap(0); }
	void DeleteOSMMap(OSMMapHandle h) { delete h; }
	
	//int addMapMarker(double lat, double lon, double course, const std::string& label = "", const std::string& html = "", double size = 5., MarkerColor = mcGreen);
	int OSMMapAddMarker(OSMMapHandle h, double lat, double lon, double course, const char* label = 0, const char* html = 0, double size = 5., int markerColor = OSMMap::mcGreen) {
		std::string l(label);
		std::string ht(html);
		OSMMap::MarkerColor color = OSMMap::mcBlue;
		switch(markerColor) {
			case 1:
				color = OSMMap::mcGold;
				break;
			case 2:
				color = OSMMap::mcGreen;
				break;
			case 3:
				color = OSMMap::mcRed;
				break;
		}
		return h->addMapMarker(lat, lon, course, l, ht, size, color);
	}
	
	bool OSMMapUpdateMarker(OSMMapHandle h, int id, double lat, double lon, double course, const char* label = 0, const char* html = 0, double size = 5., int markerColor = OSMMap::mcGreen) {
		std::string l(label);
		std::string ht(html);
		OSMMap::MarkerColor color = OSMMap::mcBlue;
		switch(markerColor) {
			case 1:
				color = OSMMap::mcGold;
				break;
			case 2:
				color = OSMMap::mcGreen;
				break;
			case 3:
				color = OSMMap::mcRed;
				break;
		}
		return h->updateMapMarker(id, lat, lon, course, l, ht, size, color);
	}	
	
	bool OSMMapDelMarker(OSMMapHandle h, int id) {
		return h->delMapMarker(id);
	}
	
	bool OSMMapExistsMarker(OSMMapHandle h, int id) {
		return h->existsMapMarker(id);
	}
	
	bool OSMMapIsLoaded(OSMMapHandle h) {
		return h->isMapLoaded();
	}
}