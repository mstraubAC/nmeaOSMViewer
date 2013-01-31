#include <QtGui>

class QWebView;
class QLineEdit;
class OSMMap;

class BrowserWindow: public QMainWindow
{
	Q_OBJECT

	public:
		BrowserWindow(const QUrl& url);
		void restartUdpListener(unsigned short udpPort);

	private:
		OSMMap* fView;
		int fPort;

	protected slots:
		void finishedLoading(bool);
		void setProgress(int p);
		void configure();
};
