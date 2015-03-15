#include "At.h"
#include "DeamonConnection.h"
#include "DeamonInterface.h"
#include "Define.h"
#include "PocketWidget.h"

PocketWidget::PocketWidget (
		QWidget *parent)
	: QLabel(parent) {

	this->pocketInternet = new QLabel(POCKET_INTERNET);
	this->pocketInternet->setAlignment (Qt::AlignCenter);

	this->copyright = new QLabel(COPYRIGHT);
	this->copyright->setAlignment (Qt::AlignCenter);

	this->softwareVersion = new QLineEdit (VERSION_NAME);
	this->softwareVersion->setReadOnly (true);
	this->softwareVersion->setAlignment (Qt::AlignCenter);

	this->warning = new QTextEdit;
	this->warning->setReadOnly (true);
	this->warning->setText (WARNING);
	this->warning->setAlignment (Qt::AlignCenter);

	this->layout = new QGridLayout;
	this->layout->addWidget (this->pocketInternet, 0, 1, 1, 2);
	this->layout->addWidget (this->copyright, 1, 1, 1, 2);
	this->layout->addWidget (this->softwareVersion, 2, 1, 1, 2);
	this->layout->addWidget (this->warning, 3, 0, 4, 4);

	this->setLayout (this->layout);

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(softwareVersionUpdate(const QString&)),
					  this,
					  SLOT(onSoftwareVersionUpdate(const QString&)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(modemPlugged (bool)),
					  this,
					  SLOT(onModemPlugged (bool)));

	QObject::connect (DeamonConnection::getInstance (),
					  SIGNAL(modemConnected (bool)),
					  this,
					  SLOT(onModemConnected (bool)));
}

void
PocketWidget::showEvent (
		QShowEvent* event) {

	DeamonInterface::getInstance ()->write (At::hardwareVersion);

	QWidget::showEvent (event);
}

bool
PocketWidget::onSoftwareVersionUpdate (
		QString value) {

	this->softwareVersion->setText (VERSION_NAME + value);

	return true;
}

bool
PocketWidget::onModemPlugged (
		bool status) {
	Q_UNUSED (status);

	return true;
}

bool
PocketWidget::onModemConnected (
		bool status) {
	Q_UNUSED (status);

	return true;
}
