#include "At.h"
#include "DeamonInterface.h"
#include "Define.h"
#include "PinVerifyDialog.h"

#include <QIcon>
#include <QPushButton>
#include <QRegExpValidator>

PinVerifyDialog::PinVerifyDialog (
		QWidget *parent)
	: QDialog(parent) {

	this->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
	this->setWindowIcon(QIcon(APPLICATION_ICON));
	this->setWindowTitle ("Verify PIN");

	this->pin = new QLineEdit;
	this->pin->setEchoMode (QLineEdit::Password);
	this->pin->setValidator (new QRegExpValidator (QRegExp("(\\d){8}"), this));

	this->attempt = new QLineEdit;
	this->attempt->setPlaceholderText ("Updating");
	this->attempt->setReadOnly (true);

	this->buttons = new QDialogButtonBox (
						QDialogButtonBox::Ok |
						QDialogButtonBox::Cancel);

	this->formLayout = new QFormLayout;
	this->formLayout->setLabelAlignment (Qt::AlignRight);
	this->formLayout->addRow ("Enter Pin Number ", this->pin);
	this->formLayout->addRow ("Attempt(s) Remaining", this->attempt);

	this->layout = new QVBoxLayout (this);
	this->layout->addLayout (this->formLayout);
	this->layout->addWidget (this->buttons);

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(pinAttemptUpdate(const QString&)),
					  this,
					  SLOT(onAttemptUpdate(const QString&)));

	QObject::connect (this->pin,
					  SIGNAL(textChanged ()),
					  this,
					  SLOT(onPinTextChanged ()));

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
PinVerifyDialog::showEvent (
		QShowEvent* event) {

	DeamonInterface::getInstance ()->write (At::pinPukAttempts);

	QDialog::showEvent (event);
}

bool
PinVerifyDialog::onPinTextChanged (
	QString text) {

	this->buttons->button (QDialogButtonBox::Ok)->setEnabled (4 <= text.length ());

	return true;
}

bool
PinVerifyDialog::onOkClicked () {

	if ( this->pin->text ().isEmpty ()) {

		QMessageBox::critical (NULL,
							   ERROR_DIALOG,
							   EMPTY_INPUT,
							   QMessageBox::Ok,
							   QMessageBox::Ok);
		return false;

	} else if  ( 4 > this->pin->text ().length ()) {
		QMessageBox::critical (NULL,
							   ERROR_DIALOG,
							   MINIMUM_LANGTH,
							   QMessageBox::Ok,
							   QMessageBox::Ok);
		return false;
	}


	DeamonInterface::getInstance ()->write (
				At::enterPin + QUOTES + this->pin->text() + QUOTES);

	DeamonInterface::getInstance ()->write (At::pinQuery);

	this->done (QDialog::Accepted);

	return true;
}

bool
PinVerifyDialog::onCancelClicked () {

	this->done (QDialog::Rejected);

	return true;
}

QString
PinVerifyDialog::getPin () {

	return this->pin->text ();
}

bool
PinVerifyDialog::onAttemptUpdate (
		QString value) {

	this->attempt->setText (value);

	return true;
}
