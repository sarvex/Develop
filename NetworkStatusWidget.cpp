#include "At.h"
#include "DeamonConnection.h"
#include "DeamonInterface.h"
#include "Define.h"
#include "NetworkStatusWidget.h"

NetworkStatusWidget::NetworkStatusWidget (
		QWidget *parent)
	: QWidget(parent) {

	this->networkAttachment << "Not Attached"
							<< "Attached";

	this->networkRegistration << "Trying to Register"
							  << "Registered"
							  << "Registration Failed"
							  << "Registration Denied"
							  << "Unknown";

	this->networkStatus << "Registered"
						<< "Registered"
						<< "Registered"
						<< "Registered"
						<< "Registered"
						<< "Registered, Roaming";

	this->networkName = new QLineEdit;
	this->networkName->setPlaceholderText ("Updating...");
	this->networkName->setReadOnly (true);

	this->rssi = new QLineEdit;
	this->rssi->setPlaceholderText ("Updating...");
	this->rssi->setReadOnly (true);

	this->cellIdentifier = new QLineEdit;
	this->cellIdentifier->setPlaceholderText ("Updating...");
	this->cellIdentifier->setReadOnly (true);

	this->csNetworkRegistration = new QLineEdit;
	this->csNetworkRegistration->setPlaceholderText ("Updating...");
	this->csNetworkRegistration->setReadOnly (true);

	this->psNetworkRegistration = new QLineEdit;
	this->psNetworkRegistration->setPlaceholderText ("Updating...");
	this->psNetworkRegistration->setReadOnly (true);

	this->psNetworkAttachment = new QLineEdit;
	this->psNetworkAttachment->setPlaceholderText ("Updating...");
	this->psNetworkAttachment->setReadOnly (true);

	this->layout = new QFormLayout;
	this->layout->setLabelAlignment (Qt::AlignRight);
	this->layout->addRow ("Network name", this->networkName);
	this->layout->addRow ("RSSI", this->rssi);
	this->layout->addRow ("Cell ID", this->cellIdentifier);
	this->layout->addRow ("CS network registration", this->csNetworkRegistration);
	this->layout->addRow ("PS network registration", this->psNetworkRegistration);
	this->layout->addRow ("PS network attachment", this->psNetworkAttachment);

	this->setLayout (this->layout);

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(networkNameUpdate(QString)),
					  this,
					  SLOT(onNetworkNameUpdate(QString)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(signalStrengthUpdate(QString)),
					  this,
					  SLOT(onRssiUpdate(QString)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(cellIdentifierUpdate(QString)),
					  this,
					  SLOT(onCellIdentifierUpdate(QString)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(csNetworkRegistrationUpdate(QString)),
					  this,
					  SLOT(onCsNetworkRegistrationUpdate(QString)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(psNetworkRegistrationUpdate(QString)),
					  this,
					  SLOT(onPsNetworkRegistrationUpdate(QString)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(psNetworkAttachmentUpdate(QString)),
					  this,
					  SLOT(onPsNetworkAttachmentUpdate(QString)));

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
NetworkStatusWidget::showEvent (
		QShowEvent* event){

	DeamonInterface::getInstance ()->write (At::networkName);
	DeamonInterface::getInstance ()->write (At::cellIdentifier);
	DeamonInterface::getInstance ()->write (At::psAttachemnt);
	DeamonInterface::getInstance ()->write (At::psRegistration);
	DeamonInterface::getInstance ()->write (At::cellIdentifier);

    QTimer::singleShot (10000, this, SLOT(onTimer ()));

    QWidget::showEvent (event);
}

bool
NetworkStatusWidget::onTimer () {
        if (true == this->networkName->text ().isEmpty ()) {
                this->networkName->setText ("Unknown");
        }

        if (true == this->rssi->text ().isEmpty ()) {
                this->rssi->setText ("Unknown");
        }

        if (true == this->cellIdentifier->text ().isEmpty ()) {
                this->cellIdentifier->setText ("Unknown");
        }

        if (true == this->csNetworkRegistration->text ().isEmpty ()) {
                this->csNetworkRegistration->setText ("Unknown");
        }

        if (true == this->psNetworkRegistration->text ().isEmpty ()) {
                this->psNetworkRegistration->setText ("Unknown");
        }

        if (true == this->psNetworkAttachment->text ().isEmpty ()) {
                this->psNetworkAttachment->setText ("Unknown");
        }


        return true;
}

bool NetworkStatusWidget::onNetworkNameUpdate (const QString& value) {


	this->networkName->setText (value);

	return true;
}

bool
NetworkStatusWidget::onRssiUpdate (
		QString value) {

	this->rssi->setText (value);

	return true;
}

bool
NetworkStatusWidget::onCellIdentifierUpdate (
		QString value) {

	this->cellIdentifier->setText (value);

	return true;
}

bool
NetworkStatusWidget::onCsNetworkRegistrationUpdate (
		QString value) {

	this->csNetworkRegistration->setText (
				this->networkStatus.at (value.toInt ()));

	return true;
}

bool
NetworkStatusWidget::onPsNetworkRegistrationUpdate (
		QString value) {

	this->psNetworkRegistration->setText (
				this->networkRegistration.at (value.toInt ()));

	return true;
}

bool
NetworkStatusWidget::onPsNetworkAttachmentUpdate (
		QString value) {

	this->psNetworkAttachment->setText (
				this->networkAttachment.at (value.toInt ()));

	return true;
}

bool
NetworkStatusWidget::onModemPlugged (
		bool status) {
	
	if (false == status) {
	
	    this->networkName->setText ("Unknown");
	    this->rssi->setText ("Unknown");
    	this->cellIdentifier->setText ("Unknown");
    	this->csNetworkRegistration->setText ("Unknown");
    	this->psNetworkRegistration->setText ("Unknown");
    	this->psNetworkAttachment->setText ("Unknown");
	}

	return true;
}

bool
NetworkStatusWidget::onModemConnected (
		bool status) {
	Q_UNUSED (status);

	return true;
}

QString
NetworkStatusWidget::readAll () {

	return QString( "Network name : " + this->networkName->text ()
					+ NEW_LINE_CARRIAGE +

					"RSSI : " + this->rssi->text ()
					+ NEW_LINE_CARRIAGE +

					"Cell ID : " + this->cellIdentifier->text ()
					+ NEW_LINE_CARRIAGE +

					"CS network registration : "
					+ this->csNetworkRegistration->text ()
					+ NEW_LINE_CARRIAGE +

					"PS network registration : "
					+ this->psNetworkRegistration->text ()
					+ NEW_LINE_CARRIAGE +

					"PS network attachment : "
					+ this->psNetworkAttachment->text ()
					+ NEW_LINE_CARRIAGE + NEW_LINE_CARRIAGE);
}

bool
NetworkStatusWidget::clear () {

	this->networkName->clear ();
	this->rssi->clear ();
	this->cellIdentifier->clear ();
	this->csNetworkRegistration->clear ();
	this->psNetworkRegistration->clear ();
	this->psNetworkAttachment->clear ();

	return true;
}
