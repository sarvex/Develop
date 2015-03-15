#include "At.h"
#include "DeamonConnection.h"
#include "DeamonInterface.h"
#include "Define.h"
#include "NetworkModeWidget.h"

#include <QAbstractButton>
#include <QMessageBox>
#include <QPushButton>

NetworkModeWidget::NetworkModeWidget (
		QWidget *parent)
	: QWidget(parent) {

	this->networkMode = new QComboBox;
	this->networkMode->addItems (QStringList () << "Dual (3G Preferred)"
												<< "2G (EDGE)"
												<< "3G");

	this->buttons = new QDialogButtonBox (
						QDialogButtonBox::Apply |
						QDialogButtonBox::Reset);

	this->buttons->button (QDialogButtonBox::Apply)->setEnabled (false);
	this->buttons->button (QDialogButtonBox::Reset)->setEnabled (false);
	this->buttons->button (QDialogButtonBox::Reset)->setText ("Refresh");

	this->formLayout = new QFormLayout;
	this->formLayout->addRow ("Network Mode", this->networkMode);

	this->layout = new QVBoxLayout;
	this->layout->addLayout (this->formLayout);
	this->layout->addStretch ();
	this->layout->addWidget (this->buttons);
	this->setLayout (this->layout);

	QObject::connect (this->networkMode,
					  SIGNAL(activated(int)),
					  this,
					  SLOT(onNetworkModeChange(int)));

	QObject::connect (this->buttons->button (QDialogButtonBox::Apply),
					  SIGNAL(clicked()),
					  this,
					  SLOT(onApplyClicked()));

	QObject::connect (this->buttons->button (QDialogButtonBox::Reset),
					  SIGNAL(clicked()),
					  this,
					  SLOT(onResetClicked()));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(getSelectedNetworkModeUpdate(const QString&)),
					  this,
					  SLOT(onGetSelectedNetworkModeUpdate(const QString&)));

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
NetworkModeWidget::showEvent (
		QShowEvent* event) {

	DeamonInterface::getInstance ()->write (At::getSelectedNetworkMode);

	QWidget::showEvent (event);
}

bool
NetworkModeWidget::onNetworkModeChange (
		int index) {

	this->buttons->button (QDialogButtonBox::Apply)->setEnabled (true);
	this->buttons->button (QDialogButtonBox::Reset)->setEnabled (0 != index);

	return true;
}

bool
NetworkModeWidget::onApplyClicked () {

	switch (this->networkMode->currentIndex ()) {

		case 0: {
			DeamonInterface::getInstance ()->write (
						At::changeNetworkMode + "0,0,0");

			QMessageBox::information (NULL,
									  "Network Mode",
									  "Mode changed to Dual (3G Preferred).",
									  QMessageBox::Ok,
									  QMessageBox::Ok);

		} break;

		case 1: {
			DeamonInterface::getInstance ()->write (
						At::changeNetworkMode + "1,0,0");

			QMessageBox::information (NULL,
									  "Network Mode",
									  "Mode changed to 2G.",
									  QMessageBox::Ok,
									  QMessageBox::Ok);
		} break;

		case 2: {
			DeamonInterface::getInstance ()->write (
						At::changeNetworkMode + "2,0,0");

			QMessageBox::information (NULL,
									  "Network Mode",
									  "Mode changed to 3G.",
									  QMessageBox::Ok,
									  QMessageBox::Ok);
		} break;
	}

	this->buttons->button (QDialogButtonBox::Apply)->setEnabled (false);
	this->buttons->button (QDialogButtonBox::Reset)->setEnabled (
				0 != this->networkMode->currentIndex ());

	emit resetNetwork ();
	return true;
}

bool
NetworkModeWidget::onResetClicked () {

	this->networkMode->setCurrentIndex (0);

	DeamonInterface::getInstance ()->write (At::changeNetworkMode + "0,0,0");

	QMessageBox::information (NULL,
							  "Network Mode",
							  "Mode changed to Dual (3G Preferred).",
							  QMessageBox::Ok,
							  QMessageBox::Ok);

	this->buttons->button (QDialogButtonBox::Apply)->setEnabled (false);
	this->buttons->button (QDialogButtonBox::Reset)->setEnabled (false);

	return true;
}

bool
NetworkModeWidget::onGetSelectedNetworkModeUpdate (
		QString value) {

	this->networkMode->setCurrentIndex (value.toInt ());

	return true;
}

bool
NetworkModeWidget::onModemPlugged (bool status) {

	if (false == status) {

		this->buttons->button (QDialogButtonBox::Apply)->setEnabled (status);
		this->buttons->button (QDialogButtonBox::Reset)->setEnabled (status);
	}
	return true;
}

bool
NetworkModeWidget::onModemConnected (bool status) {

	this->networkMode->setEnabled (false == status);

	if (status == true) {

		this->buttons->button (QDialogButtonBox::Apply)->setEnabled (false);
		this->buttons->button (QDialogButtonBox::Reset)->setEnabled (false);
	}

	return true;
}
