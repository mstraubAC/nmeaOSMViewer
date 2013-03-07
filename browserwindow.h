#include <QtGui>
#include <QSettings>
#include <map>
#include <string>
#include "gpsNMEA.h"

class QWebView;
class QLineEdit;
class QUdpSocket;
class OSMMap;

class BrowserWindow: public QMainWindow
{
	Q_OBJECT

	public:
		BrowserWindow();
		void restartUdpListener(unsigned short udpPort);

	private:
		typedef std::pair<std::string, unsigned short> PeerType;
		typedef std::map<PeerType, std::string> QueueType;
		GPSNMEA fGps;
		OSMMap* fView;
		QUdpSocket* fUDPSocket;
		QSettings fSettings;
		QueueType fUDPQueues;
		int fPort;
		int fId;

		void parseUDPQueues();

	protected slots:
		void finishedLoading(bool);
		void setProgress(int p);
		void configure();
		void processUDP();
};
