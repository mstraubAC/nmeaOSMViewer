#include <QtGui>
#include <QUdpSocket>
#include <QSettings>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <exception>
#include <boost/algorithm/string.hpp>
#include "OSMMap.h"
#include "browserwindow.h"
#include "Settings.h"
#include "gpsNMEA.h"

using namespace std;
using namespace boost::algorithm;

BrowserWindow::BrowserWindow() : fUDPSocket(0)
{
	// load resource file containing icons and html page
	QResource::registerResource("resources.rcc");

	// load settings
	fPort = fSettings.value("udpPort", 10000).toInt();

	// define OSM map display widget
	fView = new OSMMap(QUrl("qrc:///html/index.html" ), this);
	connect(fView, SIGNAL(loadFinished(bool)), SLOT(finishedLoading(bool)));
	connect(fView, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));

	// define quit routine
	QPixmap quitPix(":/icons/application-exit.png");
	QAction* quit = new QAction(quitPix, "Beenden", this);
	quit->setShortcut(tr("CTRL+Q"));
	connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()) );

	// define settings routine
	QPixmap changeSettingsPix(":/icons/preferences-other.png");
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
	restartUdpListener(fPort);
}

void BrowserWindow::restartUdpListener(unsigned short udpPort) {
	fPort = udpPort;
	fSettings.setValue("udpPort", udpPort);

	if (fUDPSocket)
		delete fUDPSocket;

	fUDPSocket = new QUdpSocket(this);
	fUDPSocket->bind(udpPort, QUdpSocket::DontShareAddress);
	connect(fUDPSocket, SIGNAL(readyRead()), this, SLOT(processUDP()));
}

void BrowserWindow::processUDP()
{
//	cout << "processUDP(): Entered" << endl;
	while (fUDPSocket->hasPendingDatagrams()) {
		QByteArray datagram;
		QHostAddress host;
		quint16 port;
		datagram.resize(fUDPSocket->pendingDatagramSize());
		fUDPSocket->readDatagram(datagram.data(), datagram.size(), &host, &port);

		PeerType p(host.toString().toStdString(), port);
		fUDPQueues[p] += string(datagram.data());

//		cout << "processUDP(): received datagram from " << p.first << ":" << p.second << " (len=" << datagram.size() << "): " << datagram.data() << endl;
	}
//	cout << "processUDP(): Done" << endl;
	parseUDPQueues();
}

void BrowserWindow::parseUDPQueues()
{
	try {
		for (QueueType::iterator itHost = fUDPQueues.begin(); itHost != fUDPQueues.end(); ++itHost) {
			unsigned int i = 0;
			cout << "Processing queue of " << itHost->first.first << ":" << itHost->first.second << endl;
			unsigned int start, len = 0;
			bool isOk = fGps.isNmeaMsg(itHost->second, &start, &len);
			while (isOk || len > 0) {
				string dgm = itHost->second.substr(start, len);
				if (!isOk && len > 0) {
					// bad dgm
					cerr << i << "(" << start << ", " << len << ") is BAD: " << dgm << endl;
				}
				else {
					// dgm ist ok
					trim(dgm);
					cout << i << "(" << start << ", " << len << "): " << dgm << endl;
					if (fGps.parseNmeaMsg(dgm)) {
						// it is s datagram which updated our position info
						stringstream ss;
						ss  << "Altitude:     " << setprecision(5) << fGps.getAltitude() << "m<br>"
							<< "Ground speed: " << setprecision(4) << fGps.getGroundSpeed() << "m/s<br>"
							<< "Sat count:    " << fGps.getSatCount() << "<br>"
							<< "Precission:   " << setprecision(1) << fGps.getHorizontalPrecision() << "m<br>";
						stringstream st;
						st  << "H=" << setprecision(5) << fGps.getAltitude() << "m / "
							<< "V=" << setprecision(4) << fGps.getGroundSpeed() << "m/s / "
							<< "S=" << fGps.getSatCount();
						if (fView->existsMapMarker(fId)) {
							cout << "Update" << endl;
							fView->updateMapMarker(fId, fGps.getLatitude(), fGps.getLongitude(), fGps.getCourse(), st.str(), ss.str());
						}
						else {
							fId = fView->addMapMarker(fGps.getLatitude(), fGps.getLongitude(), fGps.getCourse(), st.str(), ss.str());
						}
					}
				}

				// finally remove dgm from string
				try {
					itHost->second = itHost->second.substr(start+len, string::npos);
				}
				catch (const exception& e){
					cerr << "ERROR: itHost->second = itHost->second.substr(start+len, string::npos);" << endl
					     << "   -> string=" << itHost->second << endl
					     << "   -> start =" << start << endl
					     << "   -> len   =" << len << endl
					     << "   -> length=" << itHost->second.length() << endl;
				}
				i++;
				start = 0;
				len = 0;
				isOk = fGps.isNmeaMsg(itHost->second, &start, &len);
			}
		}
	}
	catch (const exception& e) {
		cerr << "Got an exception in BrowserWindo::parseUDPQueues(): " << e.what() << endl;
	}
}

void BrowserWindow::setProgress(int p) {
	if (p <= 0 || p >= 100) {
		setWindowTitle("OSM Visualisierer - Loaded");
	}
	else {
		setWindowTitle(QString("OSM Visualisierer laedt - %1%").arg(p));
	}
}
