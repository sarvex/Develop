#include "DeamonConnection.h"
#include "DeamonInterface.h"
#include "Define.h"
#include "ExitDialog.h"

#include <QApplication>
#include <QIcon>
#include <QPushButton>
#include <QProcess>

ExitDialog::ExitDialog (
		QDialog *parent)
	: QDialog(parent) {

	this->setWindowFlags (Qt::WindowSystemMenuHint |
							Qt::WindowTitleHint);
	this->setWindowIcon (QIcon (APPLICATION_ICON));


	this->message = new QLabel (EXIT_MESSAGE, this);
	this->message->setWordWrap (true);

	this->buttons = new QDialogButtonBox (QDialogButtonBox::Reset |
											QDialogButtonBox::Ok |
											QDialogButtonBox::Cancel);

	this->buttons->button(QDialogButtonBox::Reset)->setText("Shutdown Device");
	this->buttons->button(QDialogButtonBox::Ok)->setText("Quit Application");
	this->buttons->button(QDialogButtonBox::Cancel)->setText("Cancel");

	this->layout = new QVBoxLayout;
	this->layout->addWidget (this->message);
	this->layout->addWidget(this->buttons);
	this->setLayout (layout);

	QObject::connect (this->buttons->button(QDialogButtonBox::Reset),
						SIGNAL(clicked ()),
						this,
						SLOT(onShutDownDeviceClicked ()));

	QObject::connect (this->buttons->button(QDialogButtonBox::Ok),
						SIGNAL(clicked ()),
						this,
						SLOT(onQuitApplicationClicked ()));

	QObject::connect (this->buttons->button (QDialogButtonBox::Cancel),
						SIGNAL(clicked ()),
						this,
						SLOT(onCancelClicked ()));

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
ExitDialog::onShutDownDeviceClicked () {

	QProcess::startDetached (
				QApplication::applicationDirPath () + "/drivers/eject.exe",
				QStringList (),
				QApplication::applicationDirPath ());

	this->done (QDialog::Accepted);
	return true;
}

bool
ExitDialog::onQuitApplicationClicked() {

	this->done (QDialog::Accepted);
	return true;
}

bool
ExitDialog::onCancelClicked() {

	this->done (QDialog::Rejected);
	return true;
}

bool
ExitDialog::onModemPlugged (
		bool status) {

	this->buttons->button(QDialogButtonBox::Reset)->setEnabled (true == status);

	return true;
}

bool
ExitDialog::onModemConnected (
		bool status) {

	return true;
}
