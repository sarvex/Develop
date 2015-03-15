#include "At.h"
#include "Define.h"
#include "DeamonConnection.h"
#include "DeamonInterface.h"
#include "PinActivateDialog.h"
#include "PinDialog.h"
#include "PinModifyDialog.h"
#include "PinUnlockDialog.h"
#include "PinVerifyDialog.h"
#include "PinWidget.h"

PinWidget::PinWidget (
		QWidget *parent)
	: QLabel(parent) {

	this->enableLabel = new QLabel (ENABLE_LABEL);

	this->enableButton = new QPushButton("Enable");
	this->enableButton->setEnabled (false);

	this->modifyButton = new QPushButton("Modify");
	this->modifyButton->setEnabled (false);

	this->enableGroupButtons = new QDialogButtonBox;
	this->enableGroupButtons->addButton (this->enableButton,
										 QDialogButtonBox::AcceptRole);
	this->enableGroupButtons->addButton (this->modifyButton,
										 QDialogButtonBox::RejectRole);

	this->enableLayout = new QVBoxLayout;
	this->enableLayout->addWidget (this->enableLabel);
	this->enableLayout->addWidget (this->enableGroupButtons);

	this->enableGroupBox = new QGroupBox("Enable");
	this->enableGroupBox->setLayout (this->enableLayout);

	this->verifyLabel = new QLabel(VERIFY_LABEL);

	this->verifyButton = new QPushButton("Verify");
	this->verifyButton->setEnabled (false);

	this->verifyGroupButtons = new QDialogButtonBox;
	this->verifyGroupButtons->addButton (verifyButton,
										 QDialogButtonBox::AcceptRole);

	this->verifyLayout = new QVBoxLayout;
	this->verifyLayout->addWidget (this->verifyLabel);
	this->verifyLayout->addWidget (this->verifyGroupButtons);

	this->verifyGroupBox = new QGroupBox("Verify");
	this->verifyGroupBox->setLayout (this->verifyLayout);

	this->unlockLabel = new QLabel(UNLOCK_LABEL);

	this->unlockButton = new QPushButton("Unlock");
	this->unlockButton->setEnabled (false);

	this->unlockGroupButtons = new QDialogButtonBox;
	this->unlockGroupButtons->addButton (this->unlockButton,
										 QDialogButtonBox::AcceptRole);

	this->unlockLayout = new QVBoxLayout;
	this->unlockLayout->addWidget (this->unlockLabel);
	this->unlockLayout->addWidget (this->unlockGroupButtons);

	this->unlockGroupBox = new QGroupBox("Unlock");
	this->unlockGroupBox->setLayout (this->unlockLayout);

	this->layout = new QVBoxLayout;
	this->layout->addWidget (this->enableGroupBox);
	this->layout->addStretch ();
	this->layout->addWidget (this->verifyGroupBox);
	this->layout->addStretch ();
	this->layout->addWidget (this->unlockGroupBox);
	this->setLayout (this->layout);

	QObject::connect (this->enableGroupButtons,
					  SIGNAL(accepted()),
					  this,
					  SLOT(onEnableButtonClicked()));

	QObject::connect (this->enableGroupButtons,
					  SIGNAL(rejected()),
					  this,
					  SLOT(onModifyButtonClicked()));

	QObject::connect (this->verifyGroupButtons,
					  SIGNAL(accepted()),
					  this,
					  SLOT(onVerifyButtonClicked()));

	QObject::connect (this->unlockGroupButtons,
					  SIGNAL(accepted()),
					  this,
					  SLOT(onUnlockButtonClicked()));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(pinQueryUpdate(const QString&)),
					  this,
					  SLOT(onPinQueryUpdate(const QString&)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(pinCodeStatusUpdate(const QString&)),
					  this,
					  SLOT(onPinCodeUpdate(const QString&)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(incorrectPinPukUpdate(const QString&)),
					  this,
					  SLOT(onIncorrectPinPukUpdate(const QString&)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(pinModifyUpdate(const QString&)),
					  this,
					  SLOT(onPinModifyUpdate(const QString&)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(modemPlugged (bool)),
					  this,
					  SLOT(onModemPlugged (bool)));

	QObject::connect (DeamonConnection::getInstance (),
					  SIGNAL(modemConnected(bool)),
					  this,
					  SLOT(onModemConnected (bool)));
}

void
PinWidget::showEvent (
		QShowEvent* event) {

	DeamonInterface::getInstance ()->write (At::pinQuery);
	DeamonInterface::getInstance ()->write (At::pinCodeStatus);

	QWidget::showEvent (event);
}


bool
PinWidget::onPinCodeUpdate (
		QString value) {

	if (true == value.contains ("SIM PUK")) {

		this->enableButton->setText ("Disable");
		this->enableButton->setEnabled (false);

		this->modifyButton->setEnabled (false);

		this->unlockButton->setEnabled (true);

	} else if (true == value.contains ("SIM PIN")) {

		this->enableButton->setText ("Disable");
		this->enableButton->setEnabled (false);

		this->modifyButton->setEnabled (false);

		this->verifyButton->setEnabled (true);
		this->unlockButton->setEnabled (false);

	} else if (true == value.contains ("Verified")) {

		this->enableButton->setText ("Disable");
		this->enableButton->setEnabled (true);

		this->modifyButton->setEnabled (true);

		this->verifyButton->setEnabled (false);

		QMessageBox::information(NULL,
								 "Pin",
								 "Pin has been verified",
								 QMessageBox::Ok, QMessageBox::Ok);

		QTimer::singleShot (5000, DeamonInterface::getInstance (), SLOT(configure()));

	} else if (true == value.contains ("Activated")) {

		this->enableButton->setText ("Disable");
		this->enableButton->setEnabled (true);

		this->modifyButton->setEnabled (true);

		this->unlockButton->setEnabled (false);

		QMessageBox::information(NULL,
								 "Pin",
								 "Pin has been Activated",
								 QMessageBox::Ok, QMessageBox::Ok);

	}

	return true;
}

bool
PinWidget::onPinQueryUpdate (
		QString value) {

	if (value.contains ("1")) { // TODO

		this->enableButton->setText ("Disable");
		this->enableButton->setEnabled (true);

		this->modifyButton->setEnabled (true);

		this->verifyButton->setEnabled (false);

	} else {

		this->enableButton->setText ("Enable");
		this->enableButton->setEnabled ("Enable");

		this->modifyButton->setEnabled (false);

		this->verifyButton->setEnabled (false);

	}

	if (value.contains ("Pin Enabled")) {

		QMessageBox::information(NULL,
								 "Pin",
								 "Pin Enabled",
								 QMessageBox::Ok, QMessageBox::Ok);

	} else if (value.contains ("Disabled")) {

		QMessageBox::information(NULL,
								 "Pin",
								 "Pin Disabled",
								 QMessageBox::Ok, QMessageBox::Ok);
	}


	return true;
}

bool
PinWidget::onEnableButtonClicked () {

	PinDialog pin;

	if (QDialog::Accepted == pin.exec ()) {

		if (true == this->enableButton->text ().contains ("Enable")) {

			DeamonInterface::getInstance ()->write (
						At::pinServices + "1" + COMMA + QUOTES +
						pin.getPin () + QUOTES);

		} else {

			DeamonInterface::getInstance ()->write (
						At::pinServices + "0" + COMMA + QUOTES
						+ pin.getPin () + QUOTES);
		}

	}

	DeamonInterface::getInstance ()->write (At::pinQuery);
	DeamonInterface::getInstance ()->write (At::pinCodeStatus);

	return true;
}

bool
PinWidget::onModifyButtonClicked (){

	PinModifyDialog dialog;
	dialog.exec ();

	DeamonInterface::getInstance ()->write (At::pinCodeStatus);

	return true;
}

bool
PinWidget::onVerifyButtonClicked (){

	PinVerifyDialog dialog;
	dialog.exec ();

	DeamonInterface::getInstance ()->write (At::pinQuery);

	return true;
}

bool
PinWidget::onUnlockButtonClicked (){

	PinUnlockDialog dialog;
	dialog.exec ();

	DeamonInterface::getInstance ()->write (At::pinQuery);

	return true;
}


bool
PinWidget::onIncorrectPinPukUpdate (
		QString value) {

	if (true == value.contains ("incorrect PIN")) {

                QMessageBox::information(NULL,
                                         "PIN",
                                         "Pin Incorrect",
                                         QMessageBox::Ok,
                                         QMessageBox::Ok);

    } else if (true == value.contains ("incorrect PUK")) {

                QMessageBox::information(NULL,
                                         "PUK",
                                         "PUK Incorrect",
                                         QMessageBox::Ok,
                                         QMessageBox::Ok);

    } else if (true == value.contains ("SIM PUK required")) {

        this->enableButton->setEnabled (false);
        this->modifyButton->setEnabled (false);
        this->verifyButton->setEnabled (false);
        this->unlockButton->setEnabled (true);

        QMessageBox::information(NULL,
                                 "PUK Required",
                                 "Incorrect Pin. No more attempts left. Kindly enter the PUK",
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);

	} else if (value == "SIM failure"){

		QMessageBox::information(NULL,
								 "SIM Failure",
								 "Kindly insert sim in to modem.",
								 QMessageBox::Ok, QMessageBox::Ok);

	}

	return true;
}

bool
PinWidget::onPinModifyUpdate (
		QString value) {

	if(true == value.contains ("Modified")) {

		QMessageBox::information(NULL,
								 "Pin",
								 "Pin Modified",
								 QMessageBox::Ok, QMessageBox::Ok);
	}

	return true;
}

bool
PinWidget::onModemPlugged (
		bool status) {
	if (false == status) {
		this->enableButton->setEnabled (false);
		this->modifyButton->setEnabled (false);
		this->verifyButton->setEnabled (false);
		this->unlockButton->setEnabled (false);
	}

	return true;
}

bool
PinWidget::onModemConnected (
		bool status) {
	Q_UNUSED (status);

	return true;
}
