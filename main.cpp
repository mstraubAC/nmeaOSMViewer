#include <QObject>
#include <QApplication>
#include <QWidget>
#include <QtGui>
#include <QThread>
#include <QSettings>
#include <iostream>
#include <string>

#include "browserwindow.h"

using namespace std;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QCoreApplication::setOrganizationName("TechnoTeam Informationssysteme GmbH");
	QCoreApplication::setOrganizationDomain("techno-team.de");
	QCoreApplication::setApplicationName("OSMMap");

	BrowserWindow browser;
	browser.show();
	return app.exec();
}
