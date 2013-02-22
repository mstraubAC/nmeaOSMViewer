/*
 * OSMMap.h
 *
 *  Created on: Jan 31, 2013
 *      Author: marcel
 */

#ifndef OSMMAP_H_
#define OSMMAP_H_

#include <QWebView>
#include <vector>
#include "macros.h"

DLL_LOCAL class OSMMap: public QWebView
{
	Q_OBJECT

	public:
		enum MarkerColor
		{
			mcBlue = 0, mcGold = 1, mcGreen = 2, mcRed = 3
		};
		DLL_LOCAL OSMMap(QWidget* parent = 0);
		DLL_LOCAL OSMMap(const QUrl& url, QWidget* parent = 0);
		DLL_LOCAL virtual ~OSMMap();

		DLL_LOCAL int addMapMarker(double lat, double lon, double course, const std::string& label = "", const std::string& html = "", double size = 5., MarkerColor = mcGreen);
		DLL_LOCAL bool updateMapMarker(int id, double lat, double lon, double course, const std::string& label = "", const std::string& html = "", double size = 5., MarkerColor = mcGreen);
		DLL_LOCAL bool delMapMarker(int id);
		DLL_LOCAL bool existsMapMarker(int id);
		DLL_LOCAL bool isMapLoaded() const { return (fProgress == 100); }

	private:
		DLL_LOCAL QWidget* fParent;
		DLL_LOCAL QString fQuery;
		DLL_LOCAL int fProgress;
		DLL_LOCAL std::vector<bool> fIds;

		DLL_LOCAL OSMMap();
		DLL_LOCAL void init(const QUrl& url = QUrl("https://www.straub-nv.de/osmMap/"));
//		void init(const QUrl& url);
		DLL_LOCAL int getNextFreeId();

	protected slots:
		DLL_LOCAL void setProgress(int p);
		DLL_LOCAL void finishedLoading(bool);
};

#endif /* OSMMAP_H_ */
