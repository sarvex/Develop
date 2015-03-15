#include "At.h"
#include "DeamonInterface.h"
#include "Define.h"
#include "PinDialog.h"

#include <QIcon>
#include <QMessageBox>
#include <QPushButton>
#include <QRegExpValidator>

PinDialog::PinDialog (
		QWidget *parent)
	: QDialog(parent) {

	this->setWindowTitle ("PIN");
	this->setWindowIcon (QIcon (APPLICATION_ICON));
	this->setWindowFlags (Qt::WindowSystemMenuHint |
							Qt::WindowTitleHint);

	this->pin = new QLineEdit;
	this->pin->setPlaceholderText ("Pin Number");
	this->pin->setEchoMode (QLineEdit::Password);
	this->pin->setValidator (
						new QRegExpValidator (QRegExp("(\\d){8}"), this));

	this->attempt = new QLineEdit;
	this->attempt->setReadOnly (true);
	this->attempt->setPlaceholderText ("Updating...");

	this->formLayout = new QFormLayout;
	this->formLayout->setLabelAlignment (Qt::AlignRight);
	this->formLayout->addRow ("Enter Pin Number ", this->pin);
	this->formLayout->addRow ("Attempt(s) Remaining", this->attempt);

	this->buttons = new QDialogButtonBox (
						QDialogButtonBox::Ok |
						QDialogButtonBox::Cancel);

	this->buttons->button (QDialogButtonBox::Ok)->setEnabled (false);

	QObject::connect (this->pin,
					  SIGNAL(textChanged (QString)),
					  this,
					  SLOT(onPinTextChanged (QString)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(pinAttemptUpdate (QString)),
					  this,
					  SLOT(onAttemptUpdate (QString)));

	QObject::connect (this->buttons,
					  SIGNAL(accepted ()),
					  this,
					  SLOT(onOkClicked ()));

	QObject::connect (this->buttons,
					  SIGNAL(rejected ()),
					  this,
					  SLOT(onCancelClicked ()));

	this->layout = new QVBoxLayout;
	this->layout->addLayout (this->formLayout);
	this->layout->addWidget (this->buttons);

	this->setLayout (this->layout);
}

void
PinDialog::showEvent (
	QShowEvent* event) {

	DeamonInterface::getInstance ()->write (At::pinPukAttempts);

	QDialog::showEvent (event);
}

bool
PinDialog::onPinTextChanged (
	QString text) {

	this->buttons->button (QDialogButtonBox::Ok)->setEnabled (4 <= text.length ());

	return true;
 }

bool
PinDialog::onOkClicked () {

	if ( this->pin->text ().isEmpty ()) {

		QMessageBox::critical (NULL,
								ERROR_DIALOG,
								EMPTY_INPUT,
								QMessageBox::Ok,
								QMessageBox::Ok);
		return false;
	}

	DeamonInterface::getInstance ()->write (At::pinQuery);

	this->done (QDialog::Accepted);

	return true;
}

bool
PinDialog::onCancelClicked () {

	this->done (QDialog::Rejected);

	return true;
}

QString
PinDialog::getPin () {

	return this->pin->text ();
}

bool
PinDialog::onAttemptUpdate (QString value) {

	this->attempt->setText (value);

	return true;
}
