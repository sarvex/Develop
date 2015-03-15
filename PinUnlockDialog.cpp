#include <QMessageBox>

#include "At.h"
#include "DeamonInterface.h"
#include "Define.h"
#include "PinUnlockDialog.h"

#include <QRegExpValidator>

PinUnlockDialog::PinUnlockDialog (
		QWidget *parent)
	: QDialog(parent) {

	this->setWindowIcon (QIcon(APPLICATION_ICON));
	this->setWindowTitle ("Unlock PIN");
	this->setWindowFlags (Qt::WindowSystemMenuHint
						  | Qt::WindowTitleHint);

	this->currentPuk = new QLineEdit;
	this->currentPuk->setPlaceholderText ("Current PUK");
	this->currentPuk->setEchoMode (QLineEdit::Password);
	this->currentPuk->setValidator (new QRegExpValidator (QRegExp("(\\d){8}"), this));

	this->newPin = new QLineEdit;
	this->newPin->setPlaceholderText ("New PIN");
	this->newPin->setEchoMode (QLineEdit::Password);
	this->newPin->setValidator (new QRegExpValidator (QRegExp("(\\d){8}"), this));

	this->confirmPin = new QLineEdit;
	this->confirmPin->setPlaceholderText ("Confirm PIN");
	this->confirmPin->setEchoMode (QLineEdit::Password);
	this->confirmPin->setValidator (new QRegExpValidator (QRegExp("(\\d){8}"), this));

	this->attempt = new QLineEdit;
	this->attempt->setPlaceholderText ("Updating...");
	this->attempt->setReadOnly (true);

	this->formLayout = new QFormLayout;
	this->formLayout->setLabelAlignment (Qt::AlignRight);
	this->formLayout->addRow ("Current PUK", this->currentPuk);
	this->formLayout->addRow ("New PIN", this->newPin);
	this->formLayout->addRow ("Confirm PIN", this->confirmPin);
	this->formLayout->addRow ("Attempt(s) Remaining", this->attempt);

	this->buttons = new QDialogButtonBox (
						QDialogButtonBox::Ok |
						QDialogButtonBox::Cancel);

	this->layout = new QVBoxLayout;
	this->layout->addLayout (this->formLayout);
	this->layout->addWidget (this->buttons);

	this->setLayout (this->layout);

	QObject::connect (this->currentPuk,
					  SIGNAL(textChanged (QString)),
					  this,
					  SLOT(onCurrentPinTextChanged (QString)));

	QObject::connect (this->newPin,
					  SIGNAL(textChanged (QString)),
					  this,
					  SLOT(onNewPinTextChanged (QString)));

	QObject::connect (this->confirmPin,
					  SIGNAL(textChanged (QString)),
					  this,
					  SLOT(onConfirmPinTextChanged (QString)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(pukAttemptUpdate(const QString&)),
					  this,
					  SLOT(onAttemptUpdate(const QString&)));

	QObject::connect (this->buttons,
					  SIGNAL(accepted()),
					  this,
					  SLOT(onOkClicked()));

	QObject::connect (this->buttons,
					  SIGNAL(rejected()),
					  this,
					  SLOT(onCancelClicked()));
}

void
PinUnlockDialog::showEvent (
		QShowEvent* event) {

	DeamonInterface::getInstance ()->write (At::pinPukAttempts);

	QDialog::showEvent (event);
}

bool
PinUnlockDialog::onCurrentPukTextChanged (
	QString text) {

	this->buttons->button (QDialogButtonBox::Ok)->setEnabled (4 <= text.length ());

	return true;
}

bool
PinUnlockDialog::onNewPinTextChanged (
	QString text) {

	this->buttons->button (QDialogButtonBox::Ok)->setEnabled (4 <= text.length ());

	return true;
}

bool
PinUnlockDialog::onConfirmPinTextChanged (
	QString text) {

	this->buttons->button (QDialogButtonBox::Ok)->setEnabled (4 <= text.length ());

	return true;
}

QString
PinUnlockDialog::getNewPin () {

	return this->confirmPin->text ();
}

bool
PinUnlockDialog::onOkClicked (){

	if (this->currentPuk->text ().isEmpty () ||
		 this->newPin->text ().isEmpty () ||
		 this->confirmPin->text ().isEmpty () ) {

		QMessageBox::critical (
					NULL,
					ERROR_DIALOG,
					EMPTY_INPUT,
					QMessageBox::Ok, QMessageBox::Ok);
		return false;

	} else if  ( 4 > this->currentPuk->text ().length () ||
				 4 > this->newPin->text ().length () ||
				 4 > this->confirmPin->text ().length ()) {

		QMessageBox::critical (NULL,
							   ERROR_DIALOG,
							   MINIMUM_LANGTH,
							   QMessageBox::Ok, QMessageBox::Ok);
		return false;

	} else if ( this->newPin->text () != this->confirmPin->text () ) {

		QMessageBox::critical (NULL,
							   ERROR_DIALOG,
							   PIN_MISMATCH,
							   QMessageBox::Ok, QMessageBox::Ok);
		return false;
	}

	this->done (QDialog::Accepted);

	DeamonInterface::getInstance ()->write (
				At::enterPin + this->currentPuk->text ()
				+ COMMA + this->newPin->text ());

	return true;
}

bool
PinUnlockDialog::onCancelClicked (){

	this->done (QDialog::Rejected);

	return true;
}

bool
PinUnlockDialog::onAttemptUpdate (
		QString value) {

	this->attempt->setText (value);

	return true;
}
