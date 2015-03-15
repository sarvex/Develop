#include "At.h"
#include "DeamonInterface.h"
#include "Define.h"
#include "PinActivateDialog.h"

#include <QIcon>
#include <QMessageBox>
#include <QPushButton>
#include <QRegExpValidator>

PinActivateDialog::PinActivateDialog (
		QWidget *parent)
	: QDialog(parent) {

	this->setWindowIcon (QIcon (APPLICATION_ICON));
	this->setWindowTitle ("Activate PIN");
	this->setWindowFlags (Qt::WindowSystemMenuHint |
						  Qt::WindowTitleHint);

	this->pin = new QLineEdit;
	this->pin->setEchoMode (QLineEdit::Password);
	this->pin->setPlaceholderText("Pin Number");
	this->pin->setValidator (new QRegExpValidator
									(QRegExp ("(\\d){8}"), this));

	this->attempt = new QLineEdit;
	this->attempt->setPlaceholderText("Updating...");
	this->attempt->setReadOnly (true);

	this->formLayout = new QFormLayout;
	this->formLayout->setLabelAlignment (Qt::AlignRight);
	this->formLayout->addRow ("Enter Pin Number ", this->pin);
	this->formLayout->addRow ("Attempt(s) Remaining", this->attempt);

	this->buttons = new QDialogButtonBox (
						QDialogButtonBox::Ok |
						QDialogButtonBox::Cancel);

	this->buttons->button (QDialogButtonBox::Ok)->setEnabled (false);

	this->layout = new QVBoxLayout;
	this->layout->addLayout (this->formLayout);
	this->layout->addWidget (this->buttons);

	this->setLayout(this->layout);

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(pinAttemptUpdate (QString)),
					  this,
					  SLOT(onAttemptUpdate (QString)));

	QObject::connect (this->pin,
					  SIGNAL(textChanged (QString)),
					  this,
					  SLOT(onPinTextChanged (QString)));

	QObject::connect (this->buttons,
					  SIGNAL(accepted ()),
					  this,
					  SLOT(onOkClicked ()));

	QObject::connect (this->buttons,
					  SIGNAL(rejected ()),
					  this,
					  SLOT(onCancelClicked ()));
}

void
PinActivateDialog::showEvent (
	QShowEvent* event) {

	DeamonInterface::getInstance ()->write (At::pinPukAttempts);

	QDialog::showEvent (event);
}

bool
PinActivateDialog::onPinTextChanged (
	QString text) {

	this->buttons->button (QDialogButtonBox::Ok)->setEnabled (4 <= text.length ());

	return true;
 }

bool
PinActivateDialog::onOkClicked () {

	if ( this->pin->text ().isEmpty ()) {

		QMessageBox::critical (NULL,
							   ERROR_DIALOG,
							   EMPTY_INPUT,
							   QMessageBox::Ok, QMessageBox::Ok);
		return false;

	} else if  ( 4 > this->pin->text ().length ()) {

		QMessageBox::critical (NULL,
							   ERROR_DIALOG,
							   MINIMUM_LANGTH,
							   QMessageBox::Ok, QMessageBox::Ok);
		return false;
	}

	DeamonInterface::getInstance ()->write (
							At::enterPin + QUOTES +
							this->pin->text() + QUOTES);

	this->done (QDialog::Accepted);

	return true;
}

bool
PinActivateDialog::onCancelClicked () {

	if ( QMessageBox::Yes
		 == QMessageBox::information (NULL,
									  "Confirmation",
									  "Cancelling PIN or PUK verification will disable most network related functions, Continue?",
									  QMessageBox::Yes|QMessageBox::No,
									  QMessageBox::No)) {

		this->done (QDialog::Accepted);
		return true;

	} else {

		return false;
	}
}

QString
PinActivateDialog::getPin () {

	return this->pin->text ();
}

bool
PinActivateDialog::onAttemptUpdate (
	QString value) {

	this->attempt->setText (value);

	return true;
}
