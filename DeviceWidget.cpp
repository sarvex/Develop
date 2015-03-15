#include "At.h"
#include "DeamonConnection.h"
#include "DeamonInterface.h"
#include "Define.h"
#include "DeviceWidget.h"

#include <QString>

DeviceWidget::DeviceWidget (
		QWidget *parent)
	: QWidget(parent) {

	this->deviceName = new QLineEdit;
	this->deviceName->setPlaceholderText ("Updating...");
	this->deviceName->setReadOnly (true);

	this->applicationPort = new QLineEdit;
	this->applicationPort->setPlaceholderText ("Updating...");
	this->applicationPort->setReadOnly (true);

	this->serialNumber = new QLineEdit;
	this->serialNumber->setPlaceholderText ("Updating...");
	this->serialNumber->setReadOnly (true);

	this->imei = new QLineEdit;
	this->imei->setPlaceholderText ("Updating...");
	this->imei->setReadOnly (true);

	this->hardwareVersion = new QLineEdit;
	this->hardwareVersion->setPlaceholderText ("Updating...");
	this->hardwareVersion->setReadOnly (true);

	this->firmwareVersion = new QLineEdit;
	this->firmwareVersion->setPlaceholderText ("Updating...");
	this->firmwareVersion->setReadOnly (true);

	this->softwareVersion = new QLineEdit;
	this->softwareVersion->setText (VERSION_NAME);
	this->softwareVersion->setReadOnly (true);

	this->layout = new QFormLayout;
	this->layout->setLabelAlignment (Qt::AlignRight);
	this->layout->addRow ("Device Name", this->deviceName);
	this->layout->addRow ("Application Port", this->applicationPort);
	this->layout->addRow ("Serial Number", this->serialNumber);
	this->layout->addRow ("IMEI", this->imei);
	this->layout->addRow ("Hardware Version", this->hardwareVersion);
	this->layout->addRow ("Firmware Version", this->firmwareVersion);
	this->layout->addRow ("Software Version", this->softwareVersion);

	this->setLayout (this->layout);

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(deviceNameUpdate (const QString&)),
					  this,
					  SLOT(onDeviceNameUpdate (const QString&)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(interfacePortUpdate (const QString&)),
					  this,
					  SLOT(onApplicationPortUpdate (const QString&)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(serialNumberUpdate (const QString&)),
					  this,
					  SLOT(onSerialNumberUpdate(const QString&)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(imeiUpdate (const QString&)),
					  this,
					  SLOT(onImeiUpdate (const QString&)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(hardwareVersionUpdate (const QString&)),
					  this,
					  SLOT(onHardwareVersionUpdate(const QString&)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(softwareVersionUpdate (const QString&)),
					  this,
					  SLOT(onFirmwareVersionUpdate(const QString&)));

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
DeviceWidget::showEvent (
		QShowEvent* event) {

	DeamonInterface::getInstance ()->write (At::systemInformation);
	DeamonInterface::getInstance ()->write (At::hardwareVersion);
	DeamonInterface::getInstance ()->write (At::serialNumber);

    QTimer::singleShot (10000, this, SLOT(onTimer ()));

    QWidget::showEvent (event);
}

bool
DeviceWidget::onTimer () {
        if (true == this->deviceName->text ().isEmpty ()) {
                this->deviceName->setText ("Unknown");
        }

        if (true == this->applicationPort->text ().isEmpty ()) {
                this->applicationPort->setText ("Unknown");
        }

        if (true == this->serialNumber->text ().isEmpty ()) {
                this->serialNumber->setText ("Unknown");
        }

        if (true == this->imei->text ().isEmpty ()) {
                this->imei->setText ("Unknown");
        }

        if (true == this->hardwareVersion->text ().isEmpty ()) {
                this->hardwareVersion->setText ("Unknown");
        }

        if (true == this->firmwareVersion->text ().isEmpty ()) {
                this->firmwareVersion->setText ("Unknown");
        }

        if (true == this->softwareVersion->text ().isEmpty ()) {
                this->softwareVersion->setText ("Unknown");
        }


        return true;
}

bool
DeviceWidget::onDeviceNameUpdate (
		QString value) {

	this->deviceName->setText (value);

	return true;
}

bool
DeviceWidget::onApplicationPortUpdate (
		QString value) {

	this->applicationPort->setText (value);

	return true;
}

bool
DeviceWidget::onSerialNumberUpdate (
		QString value) {

	this->serialNumber->setText (value);

	return true;
}

bool
DeviceWidget::onImeiUpdate (
		QString value) {

	this->imei->setText (value);

	return true;
}

bool
DeviceWidget::onHardwareVersionUpdate (
		QString value) {

	this->hardwareVersion->setText (value);

	return true;
}

bool
DeviceWidget::onFirmwareVersionUpdate (
		QString value) {

	this->firmwareVersion->setText (value);

	return true;
}

bool
DeviceWidget::onModemPlugged (
		bool status) {
	if (false == status) {

    this->imei->setText ("Unknown");
    this->serialNumber->setText ("Unknown");
    this->applicationPort->setText ("Unknown");
    this->deviceName->setText ("Unknown");
    this->firmwareVersion->setText ("Unknown");
    this->hardwareVersion->setText ("Unknown");
	}
		
	return true;
}

bool
DeviceWidget::onModemConnected (
		bool status) {
	Q_UNUSED (status);

	return true;
}

QString
DeviceWidget::readAll () {

	return QString( "Device Name : " + this->deviceName->text ()
					+ NEW_LINE_CARRIAGE+
					"Application Port : " + this->applicationPort->text ()
					+ NEW_LINE_CARRIAGE+
					"Serial Number : " + this->serialNumber->text ()
					+ NEW_LINE_CARRIAGE+
					"IMEI : " + this->imei->text ()
					+ NEW_LINE_CARRIAGE+
					"Hardware Version : " + this->hardwareVersion->text ()
					+ NEW_LINE_CARRIAGE+
					"Firmware Version : " + this->firmwareVersion->text ()
					+ NEW_LINE_CARRIAGE+
					"Software Version : " + this->softwareVersion->text ()
					+ NEW_LINE_CARRIAGE + NEW_LINE_CARRIAGE);

}

bool
DeviceWidget::clear () {
	this->deviceName->clear ();
	this->applicationPort->clear ();
	this->serialNumber->clear ();
	this->imei->clear ();
	this->hardwareVersion->clear ();
	this->firmwareVersion->clear ();

	return true;
}
