#include "NetworkSettingWidget.h"

#include "DeamonConnection.h"
#include "DeamonInterface.h"
#include "At.h"
#include "Define.h"

NetworkSettingWidget::NetworkSettingWidget (
		QWidget *parent)
	: QWidget(parent) {

	this->mode << "Automatic"
			   << "Manual"
			   << "Deregister"
			   << ""
			   << "Manual/Automatic";

	this->apn = new QLineEdit();
	this->apn->setPlaceholderText ("Updating...");
	this->apn->setReadOnly (true);

	this->networkSelectionMode = new QLineEdit();
	this->networkSelectionMode->setPlaceholderText ("Updating...");
	this->networkSelectionMode->setReadOnly (true);

	this->layout = new QFormLayout;
	this->layout->setLabelAlignment (Qt::AlignRight);
	this->layout->addRow ("APN", this->apn);
	this->layout->addRow ("Network selection mode", this->networkSelectionMode);

	this->setLayout (this->layout);

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(accessPointNameUpdate(QString)),
					  this,
					  SLOT(onAccessPointNameUpdate(QString)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(networkSelectionModeUpdate(QString)),
					  this,
					  SLOT(onNetworkSelectionModeUpdate(QString)));

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
NetworkSettingWidget::showEvent (
		QShowEvent* event){

	DeamonInterface::getInstance ()->write (At::accessPointName);
	DeamonInterface::getInstance ()->write (At::networkName);

	QWidget::showEvent (event);
}

bool
NetworkSettingWidget::onTimer () {
        if (true == this->apn->text ().isEmpty ()) {
                this->apn->setText ("Unknown");
        }

        if (true == this->networkSelectionMode->text ().isEmpty ()) {
                this->networkSelectionMode->setText ("Unknown");
        }

        return true;
}

bool
NetworkSettingWidget::onAccessPointNameUpdate (
		QString value) {

	this->apn->setText (value);

	return true;
}

bool
NetworkSettingWidget::onNetworkSelectionModeUpdate (
		QString value) {

	this->networkSelectionMode->setText (this->mode.at(value.toInt ()));

	return true;
}

bool
NetworkSettingWidget::onModemPlugged (
		bool status) {
	if (false == status) {
	
		this->apn->setText ("Unknown");
	    this->networkSelectionMode->setText ("Unknown");
	}

	return true;
}

bool
NetworkSettingWidget::onModemConnected (
		bool status) {
	Q_UNUSED (status);

	return true;
}

QString
NetworkSettingWidget::readAll () {

	return QString( "APN : " + this->apn->text () + NEW_LINE_CARRIAGE+
					"Network selection mode : " +
					this->networkSelectionMode->text ()
					+ NEW_LINE_CARRIAGE + NEW_LINE_CARRIAGE);
}

bool
NetworkSettingWidget::clear () {

	this->apn->clear ();
	this->networkSelectionMode->clear ();

	return true;
}
