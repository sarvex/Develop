#include "At.h"
#include "DeamonInterface.h"

#include "Define.h"
#include "PinModifyDialog.h"

#include <QIcon>
#include <QMessageBox>
#include <QPushButton>
#include <QRegExpValidator>

PinModifyDialog::PinModifyDialog (
		QWidget *parent)
	: QDialog(parent) {

	this->setWindowTitle ("Modify PIN");
	this->setWindowIcon (QIcon (APPLICATION_ICON));
	this->setWindowFlags (Qt::WindowSystemMenuHint |
							Qt::WindowTitleHint);

	this->validator = new QRegExpValidator (QRegExp("(\\d){8}"), this);

	this->currentPin = new QLineEdit;
	this->currentPin->setPlaceholderText ("Current PIN...");
	this->currentPin->setEchoMode (QLineEdit::Password);
	this->currentPin->setValidator (this->validator);

	this->newPin = new QLineEdit;
	this->newPin->setPlaceholderText ("New PIN...");
	this->newPin->setEchoMode (QLineEdit::Password);
	this->newPin->setValidator (this->validator);

	this->confirmPin = new QLineEdit;
	this->confirmPin->setPlaceholderText ("Confirm PIN...");
	this->confirmPin->setEchoMode (QLineEdit::Password);
	this->confirmPin->setValidator (this->validator);

	this->attempt = new QLineEdit;
	this->attempt->setPlaceholderText ("Updating...");
	this->attempt->setReadOnly (true);

	this->buttons = new QDialogButtonBox (
						QDialogButtonBox::Ok |
						QDialogButtonBox::Cancel);

	this->buttons->button (QDialogButtonBox::Ok)->setEnabled (false);

	this->formLayout = new QFormLayout;
	this->formLayout->setLabelAlignment (Qt::AlignRight);
	this->formLayout->addRow ("Current PIN", this->currentPin);
	this->formLayout->addRow ("New PIN", this->newPin);
	this->formLayout->addRow ("Confirm PIN", this->confirmPin);
	this->formLayout->addRow ("Attempt(s) Remaining", this->attempt);

	this->layout = new QVBoxLayout;
	this->layout->addLayout (this->formLayout);
	this->layout->addWidget (this->buttons);

	this->setLayout (this->layout);

	QObject::connect (this->currentPin,
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
					  SIGNAL(pinAttemptUpdate(const QString&)),
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
PinModifyDialog::showEvent (
		QShowEvent* event) {

	DeamonInterface::getInstance ()->write (At::pinPukAttempts);

	QDialog::showEvent (event);
}

QString
PinModifyDialog::getNewPin () {

	return this->confirmPin->text ();
}

bool
PinModifyDialog::onCurrentPinTextChanged (
	QString text) {

	this->buttons->button (QDialogButtonBox::Ok)->setEnabled (4 <= text.length ());

	return true;
}

bool
PinModifyDialog::onNewPinTextChanged (
	QString text) {

	this->buttons->button (QDialogButtonBox::Ok)->setEnabled (4 <= text.length ());

	return true;
}

bool
PinModifyDialog::onConfirmPinTextChanged (
	QString text) {

	this->buttons->button (QDialogButtonBox::Ok)->setEnabled (4 <= text.length ());

	return true;
}

bool
PinModifyDialog::onOkClicked (){

	if ( this->currentPin->text ().isEmpty () ||
		 this->newPin->text ().isEmpty () ||
		 this->confirmPin->text ().isEmpty () ) {

		QMessageBox::critical (NULL,
							   ERROR_DIALOG,
							   EMPTY_INPUT,
							   QMessageBox::Ok,
							   QMessageBox::Ok);
		return false;

	} else if  ( 4 > this->currentPin->text ().length () ||
				 4 > this->newPin->text ().length () ||
				 4 > this->confirmPin->text ().length ()) {

		QMessageBox::critical (NULL,
							   ERROR_DIALOG,
							   MINIMUM_LANGTH,
							   QMessageBox::Ok,
							   QMessageBox::Ok);

		return false;

	} else if ( this->newPin->text () != this->confirmPin->text () ) {

		QMessageBox::critical (NULL,
							   ERROR_DIALOG,
							   PIN_MISMATCH,
							   QMessageBox::Ok,
							   QMessageBox::Ok);

		return false;
	}

	this->done (QDialog::Accepted);

	DeamonInterface::getInstance ()->write (
				At::pinModify + this->currentPin->text () + COMMA +
				this->newPin->text ());

	return true;
}

bool
PinModifyDialog::onCancelClicked (){

	this->done (QDialog::Rejected);

	return true;
}

bool
PinModifyDialog::onAttemptUpdate (
		const QString& value) {

	this->attempt->setText (value);

	return true;
}
