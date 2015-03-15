#include "At.h"
#include "DeamonConnection.h"
#include "DeamonInterface.h"
#include "OperatorWidget.h"

#include <QFileDialog>
#include <QTextStream>

OperatorWidget::OperatorWidget (
		QWidget *parent)
	: QWidget(parent) {

	this->tabs = new QTabWidget;

	this->deviceWidget = new DeviceWidget;
	this->simUsimWidget = new SimUsimWidget;
	this->networkStatusWidget = new NetworkStatusWidget;
	this->networkSettingWidget = new NetworkSettingWidget;

	this->tabs->addTab (this->deviceWidget, "Device");
	this->tabs->addTab (this->simUsimWidget, "SIM / USIM");
	this->tabs->addTab (this->networkStatusWidget, "Network Status");
	this->tabs->addTab (this->networkSettingWidget, "Network Settings");

	this->buttons = new QDialogButtonBox (
						  QDialogButtonBox::Save |
						  QDialogButtonBox::Reset);

	this->buttons->button (QDialogButtonBox::Reset)->setText ("Refresh");

	this->layout = new QVBoxLayout;
	this->layout->setContentsMargins (0, 0, 0, 0);
	this->layout->addWidget (this->tabs);
	this->layout->addWidget (this->buttons);

	this->setLayout (this->layout);

	QObject::connect (this->buttons->button (QDialogButtonBox::Save),
					  SIGNAL(clicked()),
					  this,
					  SLOT(onSaveClicked()));

	QObject::connect (this->buttons->button (QDialogButtonBox::Reset),
					  SIGNAL(clicked()),
					  this,
					  SLOT(onResetClicked()));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(modemPlugged (bool)),
					  this,
					  SLOT(onModemPlugged (bool)));

	QObject::connect (DeamonConnection::getInstance (),
					  SIGNAL(modemConnected (bool)),
					  this,
					  SLOT(onModemConnected (bool)));

}

bool
OperatorWidget::onSaveClicked () {

	QString fileName = QFileDialog::getSaveFileName (this,
													"Save Settings",
													QDir::tempPath (),
													"Text Files (*.txt)");
	if (true == fileName.isEmpty ()) {

		return false;
	}

	QFile file(fileName);

	file.open (QIODevice::WriteOnly | QIODevice::Truncate);
	QTextStream textStream(&file);

	textStream << this->deviceWidget->readAll ();
	textStream << this->simUsimWidget->readAll ();
	textStream << this->networkStatusWidget->readAll ();
	textStream << this->networkSettingWidget->readAll ();

	file.close();

	return true;
}

bool
OperatorWidget::onResetClicked () {

	this->buttons->setEnabled (false);

	DeamonInterface::getInstance ()->write (At::systemInformation);
	DeamonInterface::getInstance ()->write (At::hardwareVersion);
	DeamonInterface::getInstance ()->write (At::messageCenterNumber);
	DeamonInterface::getInstance ()->write (At::pinCodeStatus);
	DeamonInterface::getInstance ()->write (At::messageCount);
	DeamonInterface::getInstance ()->write (At::contactCount);
	DeamonInterface::getInstance ()->write (At::ownNumber);
	DeamonInterface::getInstance ()->write (At::accessPointName);
	DeamonInterface::getInstance ()->write (At::networkName);
	DeamonInterface::getInstance ()->write (At::cellIdentifier);
	DeamonInterface::getInstance ()->write (At::psAttachemnt);
	DeamonInterface::getInstance ()->write (At::psRegistration);
	DeamonInterface::getInstance ()->write (At::cellIdentifier);

	this->buttons->setEnabled (false);

	this->deviceWidget->clear ();
	this->simUsimWidget->clear ();
	this->networkStatusWidget->clear ();
	this->networkSettingWidget->clear ();

	return true;
}

bool
OperatorWidget::onModemPlugged (
		bool status) {

	this->buttons->setEnabled (status);

	return true;
}

bool
OperatorWidget::onModemConnected (
		bool status) {
	Q_UNUSED (status)

	return true;
}
