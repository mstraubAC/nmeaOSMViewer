#include <QtGui>
#include <QtWebKit>
#include <iostream>
#include <sstream>
#include "OSMMap.h"
#include "browserwindow.h"
#include "Settings.h"
#include "gpsNMEA.h"

using namespace std;

BrowserWindow::BrowserWindow(const QUrl& url)
{
	// define OSM map display widget
	fView = new OSMMap(this);
	connect(fView, SIGNAL(loadFinished(bool)), SLOT(finishedLoading(bool)));
	connect(fView, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));

	// define quit routine
	QPixmap quitPix("application-exit.png");
	QAction* quit = new QAction(quitPix, "Beenden", this);
	quit->setShortcut(tr("CTRL+Q"));
	connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()) );

	// define settings routine
	QPixmap changeSettingsPix("preferences-other.png");
	QAction* changeSettings = new QAction(changeSettingsPix, "Einstellungen", this);
	changeSettings->setShortcut(tr("CTRL+S"));
	connect(changeSettings, SIGNAL(triggered()), SLOT(configure()));

	QToolBar* toolBar = addToolBar(tr("main toolbar"));
	toolBar->addAction(quit);
	toolBar->addAction(changeSettings);

	QMenu* file = menuBar()->addMenu("Datei");
	file->addAction(changeSettings);
	file->addSeparator();
	file->addAction(quit);

	setCentralWidget(fView);
	setUnifiedTitleAndToolBarOnMac(true);

	statusBar()->showMessage("Waiting for NMEA datagrams");
}

void BrowserWindow::configure()
{
	Settings* s = new Settings(this);
	s->setAttribute(Qt::WA_DeleteOnClose);
	s->show();
}

void BrowserWindow::finishedLoading(bool)
{
	cout << "Browser finished loading." << endl;

	string fNMEA = "$GPRMC,174455,A,5124.631,N,00642.235,E,000.0,360.0,090301,000.6,W*68";
	GPSNMEA gps;
	bool nmeaOk = gps.isNmeaMsg(fNMEA);
	gps.parseNmeaMsg(fNMEA);
	cout << gps << endl;

	fView->addMapMarker(gps.getLatitude(), gps.getLongitude(), gps.getCourse(), "test");


//	fView->page()->mainFrame()->evaluateJavaScript(jQuer);
}

void BrowserWindow::restartUdpListener(unsigned short udpPort) {
	cout << "BrowserWindow::restartUdpListener" << endl;
}

void BrowserWindow::setProgress(int p) {
	if (p <= 0 || p >= 100) {
		setWindowTitle("OSM Visualisierer - Loaded");
	}
	else {
		setWindowTitle(QString("OSM Visualisierer laedt - %1%").arg(p));
	}
}
