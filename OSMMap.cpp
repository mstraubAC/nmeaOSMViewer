/*
 * OSMMap.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: marcel
 */

#include <QtGui>
#include <QtWebKit>
#include <iostream>
#include <sstream>
#include "OSMMap.h"

using namespace std;

OSMMap::OSMMap() : fParent(0), fProgress(0)
{
	init();
}

OSMMap::OSMMap(QWidget* parent) : QWebView(parent), fParent(parent), fProgress(0)
{
	init();
}

void OSMMap::init(const QUrl& url)
{
	fIds.clear();
	fIds.resize(100, false);

	QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
	QNetworkProxyFactory::setUseSystemConfiguration(true);

	setContextMenuPolicy(Qt::NoContextMenu);

	// connect to signals
	connect(this, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
	connect(this, SIGNAL(loadFinished(bool)), SLOT(finishedLoading(bool)));

	load(url);
}

int OSMMap::getNextFreeId()
{
	int id = -1;
	for (unsigned int i = 0; i < fIds.size(); ++i) {
		if (!fIds[i]) {
			fIds[i] = true;
			id = i;
			break;
		}
	}
	return id;
}

OSMMap::~OSMMap()
{
	// TODO Auto-generated destructor stub
}

void OSMMap::setProgress(int p)
{
	fProgress = p;
}

void OSMMap::finishedLoading(bool)
{
	setProgress(100);
}

int OSMMap::addMapMarker(double lat, double lon, double course,
		const std::string& label, const std::string& html, double size, MarkerColor color)
{
	if (!isMapLoaded()) {
		cerr << "BrowserWindow::addMapMarker: Browser window is not yet loaded!!!" << endl;
		return 0;
	}
	int id = getNextFreeId();
	cout << "id: " << id << endl;
	if (id < 0) return id;


	string scolor = "img/marker.png";
	switch (color) {
		case mcBlue:
			scolor = "img/marker-blue.png";
			break;
		case mcGreen:
			scolor = "img/marker-green.png";
			break;
		case mcGold:
			scolor = "img/marker-Gold.png";
			break;
		case mcRed:
			scolor = "img/marker.png";
			break;
	}
	/* Parameter description for javascript call
	 * 1. id
	 * 2. lat
	 * 3. lon
	 * 4. course
	 * 5. html
	 * 6. label
	 * 7. marker size
	 * 8. marker color
	 */
	QString jsAddMarkerTpl = tr("createMapMarker(%1, %2, %3, %4, '%5', '%6', '%7', '%8');");
	QString jsAddMarker = jsAddMarkerTpl.arg(id).arg(lat).arg(lon).arg(course).arg(html.c_str()).arg(label.c_str()).arg(size).arg(scolor.c_str());
	cout << "addMapMarker: " << jsAddMarker.toAscii().constData() << endl;
	QVariant r = page()->mainFrame()->evaluateJavaScript(jsAddMarker);
	cout << " --> result: " <<  r.toString().toLocal8Bit().constData() << endl;

	return id;
}
