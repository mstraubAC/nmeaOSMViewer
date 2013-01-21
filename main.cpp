#include <QApplication>
#include <QWidget>
#include <QtGui>

#include "browserwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	QUrl url;
	if (argc > 1) {
		url = QUrl(argv[1]);
	}
	else {
		url = QUrl("https://www.straub-nv.de/osmMap/");
	}

	BrowserWindow browser(url);
	browser.show();

    return app.exec();
}