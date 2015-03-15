#include "At.h"
#include "CheckBalanceWidget.h"
#include "DeamonConnection.h"
#include "DeamonInterface.h"
#include "Define.h"

CheckBalanceWidget::CheckBalanceWidget(
		QWidget *parent)
	: QLabel(parent) {

	this->prepaidLabel = new QLabel(tr("Prepaid Account Balance"), this);

	this->checkButton = new QPushButton(tr("Check"), this);

	this->serviceLine = new QLineEdit(this);
	this->serviceLine->setPlaceholderText (tr("Enter Service Shortcode"));
	this->serviceLine->setProperty ("class", "serviceLine");

	this->sendButton = new QPushButton(tr("Send"), this);

	this->serviceText = new QTextEdit(this);
	this->serviceText->setReadOnly (true);
	this->serviceText->setProperty ("class", "serviceText");

	this->layout = new QGridLayout;
	this->layout->setContentsMargins (0, 0, 0, 0);
	this->layout->addWidget (this->prepaidLabel, 0, 0, 1, 5);
	this->layout->addWidget (this->checkButton, 0, 5, 1, 1);
	this->layout->addWidget (this->serviceLine, 1, 0, 1, 5);
	this->layout->addWidget (this->sendButton, 1, 5, 1, 1);
	this->layout->addWidget (this->serviceText, 2, 0, 8, 6);

	this->setLayout (this->layout);

	QObject::connect (this->checkButton,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onCheckButtonClicked()));

	QObject::connect (this->serviceLine,
					  SIGNAL(textChanged(const QString &)),
					  this,
					  SLOT(onServiceLineTextChanged(const QString &)));

	QObject::connect (this->sendButton,
					  SIGNAL(clicked()),
					  this,
					  SLOT(sendServiceCode()));

	QObject::connect (this->serviceLine,
					  SIGNAL(returnPressed()),
					  this->sendButton,
					  SIGNAL(clicked()) );

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(checkBalanceUpdate (QString)),
					  SLOT(onServiceTextUpdate (QString)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(incorrectPinPukUpdate(QString)),
					  this,
					  SLOT(onServiceTextUpdate(QString)));

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
CheckBalanceWidget::showEvent (
		QShowEvent* event){

	this->checkButton->setEnabled (true);
	this->sendButton->setEnabled (false);

	this->serviceText->setText ("");

	QWidget::showEvent (event);
}

bool
CheckBalanceWidget::onServiceLineTextChanged (
		QString text ){

	this->sendButton->setEnabled (!text.isEmpty());

	return true;
}

bool
CheckBalanceWidget::onCheckButtonClicked() {

	this->checkButton->setEnabled (false);

	DeamonInterface::getInstance ()->write (
				At::supplementaryServices + BALANCE_NUMBER +
				QUOTES + COMMA + "15");

	this->serviceText->setFocus ();
	this->serviceText->setText (this->serviceText->toPlainText ()
								+ "Send : " + BALANCE_NUMBER + CARRIAGE_RETURN);

	return true;
}

bool
CheckBalanceWidget::sendServiceCode () {

	this->sendButton->setEnabled (false);

	DeamonInterface::getInstance ()->write (
				At::supplementaryServices + this->serviceLine->text ()
				+ QUOTES + COMMA + "15");

	this->serviceText->setFocus ();
	this->serviceText->setText (this->serviceText->toPlainText ()
								+ "Send : " + this->serviceLine->text ()
								+ CARRIAGE_RETURN);

	this->serviceLine->clear ();

	return true;
}

bool
CheckBalanceWidget::onServiceTextUpdate(
		QString value){

	if( (value.contains("retry operation")) || value.contains ("unknown")) {

		this->serviceText->setText (this->serviceText->toPlainText ()
									+ COMMAND_PROMPT + tr("Received : Failed")
									+ CARRIAGE_RETURN);

	}  else {

		this->serviceText->setText (this->serviceText->toPlainText ()
									+ COMMAND_PROMPT + value + CARRIAGE_RETURN);
	}

	this->checkButton->setEnabled (true);

	return true;
}

bool
CheckBalanceWidget::onModemPlugged (
		bool status) {

	this->checkButton->setEnabled (true == status);
	this->serviceLine->setReadOnly (false == status);

	if (false == status) {

		this->sendButton->setEnabled (true == status);
		this->serviceLine->clear ();

	} else {

	}
	return true;
}

bool
CheckBalanceWidget::onModemConnected (
		bool status) {
	Q_UNUSED (status);

    this->serviceLine->clear ();
    this->serviceLine->setReadOnly (true);

    return true;
}
