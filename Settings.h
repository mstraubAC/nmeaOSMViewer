/*
 * Settings.h
 *
 *  Created on: Jan 24, 2013
 *      Author: marcel
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <qwidget.h>
class QSpinBox;

class Settings: public QWidget
{
	Q_OBJECT

	public:
		Settings();
		Settings(QWidget* parent);
		virtual ~Settings();

	protected slots:
		void OnOkPressed();
		void OnCancelPressed();

	private:
		void InitWidget();
		QWidget* fParent;
		QSpinBox* fSbUdpPortnumber;

};

#endif /* SETTINGS_H_ */
