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

class OSMMap: public QWebView
{
	Q_OBJECT

	public:
		enum MarkerColor
		{
			mcBlue = 0, mcGold = 1, mcGreen = 2, mcRed = 3
		};
		OSMMap(QWidget* parent = 0);
		OSMMap(const QUrl& url, QWidget* parent = 0);
		virtual ~OSMMap();

		int addMapMarker(double lat, double lon, double course, const std::string& label = "", const std::string& html = "", double size = 5., MarkerColor = mcGreen);
		bool updateMapMarker(int id, double lat, double lon, double course, const std::string& label = "", const std::string& html = "", double size = 5., MarkerColor = mcGreen);
		bool delMapMarker(int id);
		bool existsMapMarker(int id);
		bool isMapLoaded() const { return (fProgress == 100); }

	private:
		QWidget* fParent;
		QString fQuery;
		int fProgress;
		std::vector<bool> fIds;

		OSMMap();
		void init(const QUrl& url = QUrl("https://www.straub-nv.de/osmMap/"));
//		void init(const QUrl& url);
		int getNextFreeId();

	protected slots:
		void setProgress(int p);
		void finishedLoading(bool);
};

#endif /* OSMMAP_H_ */
