#include "DeamonConnection.h"
#include "DeamonInterface.h"
#include "Define.h"
#include "TimeLabel.h"

#include <QDateTime>
#include <QLabel>
#include <QPainter>
#include <QRect>
#include <QDebug>

TimeLabel::TimeLabel (
		QWidget *parent)
	: QLabel(parent) {

	this->start.start ();

	this->timer = new QTimer;

	QObject::connect (this->timer,
					  SIGNAL(timeout()),
					  this,
					  SLOT(onTimer()));

	QObject::connect (DeamonConnection::getInstance (),
					  SIGNAL(modemConnected (bool)),
					  this,
					  SLOT(onModemConnected (bool)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(modemPlugged (bool)),
					  this,
					  SLOT(onModemPlugged (bool)));

	this->setVisible (false);
}

void
TimeLabel::paintEvent (
		QPaintEvent * event) {
	Q_UNUSED (event);

	QPainter painter(this);
	painter.setRenderHint (QPainter::Antialiasing);
	painter.setPen (Qt::white);
	painter.drawText (QRect(0,0, this->width (), this->height ()),
					  Qt::AlignRight, this->text ());
}

bool
TimeLabel::onTimer () {

	this->duration = QTime (0,0).addMSecs (this->start.elapsed ());

	this->setText (this->duration.toString (TIME_FORMAT));

	return true;
}

bool
TimeLabel::onModemConnected (bool status) {

	this->setVisible (status);

	if (true == status) {

		this->start.restart ();
		this->timer->start (1000);

	} else {

		this->timer->stop ();
	}
	return true;
}

bool
TimeLabel::onModemPlugged (
		bool status) {
	Q_UNUSED (status);

	return true;
}
