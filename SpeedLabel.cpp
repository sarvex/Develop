#include "DeamonConnection.h"
#include "DeamonInterface.h"
#include "SpeedLabel.h"

#include <QDateTime>
#include <QLabel>
#include <QPainter>
#include <QRect>

SpeedLabel::SpeedLabel(
		QWidget *parent)
	: QLabel(parent) {

	QObject::connect (DeamonConnection::getInstance (),
					  SIGNAL(speedUpdate(double, double)),
					  this,
					  SLOT(onSpeedUpdate(double, double)));

	QObject::connect (DeamonConnection::getInstance (),
					  SIGNAL(modemConnected (bool)),
					  this,
					  SLOT(onModemConnected (bool)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(modemPlugged (bool)),
					  this,
					  SLOT(onModemPlugged (bool)));
}

void
SpeedLabel::paintEvent (
		QPaintEvent * event) {
	Q_UNUSED (event);

	QPainter painter(this);
	painter.setRenderHint (QPainter::Antialiasing);
	painter.setPen (Qt::white);
	painter.drawText (QRect(0,0, this->width (), this->height ()),
					  Qt::AlignCenter, this->text ());
}

bool
SpeedLabel::onSpeedUpdate (double upload, double download) {
	this->setText("Upload / Download Speed : "
				  + QString::number (upload, 'f', 2) + " KBps / "
				  + QString::number (download, 'f', 2) + " KBps");

	return true;
}

bool
SpeedLabel::onModemConnected (bool status) {

	this->setVisible (status);

	return true;
}

bool
SpeedLabel::onModemPlugged (
		bool status) {
	Q_UNUSED (status);

	return true;
}
