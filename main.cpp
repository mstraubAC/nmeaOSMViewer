#include <QObject>
#include <QApplication>
#include <QWidget>
#include <QtGui>
#include <QThread>
#include <iostream>
#include <string>

#include "browserwindow.h"

using namespace std;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	BrowserWindow browser;
	browser.show();
	return app.exec();
}
