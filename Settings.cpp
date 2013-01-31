/*
 * Settings.cpp
 *
 *  Created on: Jan 24, 2013
 *      Author: marcel
 */

#include "Settings.h"
#include "browserwindow.h"
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPixmap>
#include <QIcon>
#include <QPushButton>
#include <iostream>

using namespace std;

Settings::Settings() : fParent(0)
{
	InitWidget();
}

Settings::Settings(QWidget* parent) : fParent(parent) {
	InitWidget();
}

void Settings::InitWidget() {
	setWindowTitle("NMEA OSM Viewer - Settings");


	// input field for the udp listener port number
	QLabel* lUdpPortnumber = new QLabel("UDP Portnummer: ", this);
	QSpinBox* sbUdpPortnumber = new QSpinBox(this);

	// define cancel and ok/apply button
	QPixmap pmCancel(":/icons/dialog-cancel.png");
	QIcon iCancel(pmCancel);
	QPushButton* btnCancel = new QPushButton("Abbrechen", this);
	btnCancel->setIcon(iCancel);
	btnCancel->setIconSize(pmCancel.rect().size());
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(OnCancelPressed()));

	QPixmap pmOk(":/icons/dialog-ok-apply.png");
	QIcon iOk(pmOk);
	QPushButton* btnOk = new QPushButton("Ok", this);
	btnOk->setIcon(iOk);
	btnOk->setIconSize(pmOk.rect().size());
	connect(btnOk, SIGNAL(clicked()), this, SLOT(OnOkPressed()));

	// choose grid layout manger and add all out widgets
	QGridLayout* grid = new QGridLayout();
	grid->addWidget(lUdpPortnumber, 0, 0);
	grid->addWidget(sbUdpPortnumber, 0, 1);
	grid->addWidget(btnCancel, 1, 0);
	grid->addWidget(btnOk, 1, 1);

	setLayout(grid);
}

Settings::~Settings()
{
	// TODO Auto-generated destructor stub
}

void Settings::OnOkPressed()
{
	try {
		BrowserWindow* browser = dynamic_cast<BrowserWindow*>(fParent);
		browser->restartUdpListener(900);
	}
	catch (const std::bad_cast& e) {
		cerr << "Settings::OnPressed: " << e.what()<< endl;
	}
	this->close();
}

void Settings::OnCancelPressed()
{
	this->close();
}
