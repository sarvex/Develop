#include "At.h"
#include "ContactMultipleSelectDialog.h"
#include "Define.h"
#include "DeamonInterface.h"
#include "DeamonSetting.h"
#include "MessageDialog.h"

#include <QMessageBox>
#include <QValidator>

MessageDialog::MessageDialog (
		const QString &number,
		const QString &content,
		QWidget *parent)
	: QDialog(parent){

	this->initialize ();

	this->number->setText (number);
	this->content->setText (content);
}

MessageDialog::MessageDialog (
		QWidget *parent)
	: QDialog(parent) {

	this->initialize ();
}

bool
MessageDialog::initialize () {

	this->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
	this->setWindowIcon(QIcon(APPLICATION_ICON));

	this->sendTo = new QPushButton ("Send To");

	this->number = new QLineEdit;
	this->number->setPlaceholderText ("Destination Number;");
	 this->number->setValidator(new QRegExpValidator(QRegExp("(\\+?\\d*\\;)*"), this));
	this->number->setToolTip (
				"For multiple messages, separate numbers by semi-colon");

	this->characterCount = new QLabel("0 Character");
	this->messageCount = new QLabel ("0 Message");

	this->content = new QTextEdit;

	this->buttons = new QDialogButtonBox (
						QDialogButtonBox::Yes |
						QDialogButtonBox::Save |
						QDialogButtonBox::Cancel);

	this->buttons->button (QDialogButtonBox::Yes)->setText ("Send");

	this->buttons->button (QDialogButtonBox::Yes)->setEnabled (false);
	this->buttons->button (QDialogButtonBox::Save)->setEnabled (false);

	this->layout = new QGridLayout;
	this->layout->addWidget (this->sendTo, 0, 0, 1, 1);
	this->layout->addWidget (this->number, 0, 1, 1, 3);
	this->layout->addWidget (this->characterCount, 1, 0, 1, 2);
	this->layout->addWidget (this->messageCount, 1, 2, 1, 2);
	this->layout->addWidget (this->content, 2, 0, 8, 4);
	this->layout->addWidget (this->buttons, 10, 0, 1, 4);

	this->setLayout (this->layout);

	QObject::connect (this->sendTo,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onSendToClicked()));

	QObject::connect (this->number,
					  SIGNAL(textChanged(const QString&)),
					  this,
					  SLOT(onNumberTextChanged(const QString&)));

	QObject::connect (this->content,
					  SIGNAL(textChanged()),
					  this,
					  SLOT(onContentTextChanged()));

	QObject::connect (this->buttons->button (QDialogButtonBox::Yes),
					  SIGNAL(clicked()),
					  this,
					  SLOT(onSendClicked()));

	QObject::connect (this->buttons->button (QDialogButtonBox::Save),
					  SIGNAL(clicked()),
					  this,
					  SLOT(onSaveClicked()));

	QObject::connect (this->buttons->button (QDialogButtonBox::Cancel),
					  SIGNAL(clicked()),
					  this,
					  SLOT(onCancelClicked()));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(modemPlugged()),
					  this,
					  SLOT(onModemPlugged ()));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(modemUnplugged()),
					  this,
					  SLOT(onModemUnplugged ()));

	return true;
}

bool
MessageDialog::onNumberTextChanged (
		QString number) {

	this->buttons->button (QDialogButtonBox::Yes)->setEnabled (
				false == number.isEmpty ());

	this->buttons->button (QDialogButtonBox::Save)->setEnabled (
				false == number.isEmpty ());

	return true;
}

bool
MessageDialog::onContentTextChanged () {

	if ( MESSAGE_LENGTH < this->content->toPlainText ().size ()) {

		this->content->setText (this->data);
		return false;

	} else {

		this->data = this->content->toPlainText ();
	}

	this->buttons->button (QDialogButtonBox::Save)->setEnabled (
				(false == this->content->toPlainText ().isEmpty ())
				|| (true == this->buttons->button (
						QDialogButtonBox::Yes)->isEnabled ()));

	int count = this->content->toPlainText ().size ();
	if ( 1 == count) {

		this->characterCount->setText ("1 Character");
	} else {

		this->characterCount->setText (QString::number (count) + " Characters");
	}

	int message = (count / MESSAGE_LENGTH);
	if (count % MESSAGE_LENGTH) {

		message++;
	}

	if (1 == message) {

		this->messageCount->setText ("1 Message");

	} else {

		this->messageCount->setText (QString::number (message) + " Messages");
	}

	return true;
}

bool
MessageDialog::onSendToClicked () {

	ContactMultipleSelectDialog contacts;

	if (QDialog::Accepted == contacts.exec ()) {

		QStringList list = contacts.getNumbers ();
		this->number->setText (list.join (SEMI_COLON));

		return true;

	} else {

		return false;
	}
}

bool
MessageDialog::onSendClicked () {

	this->createMessages (
				DeamonSetting::getInstance ()->getMessageStorage ()
				? Message::Outbox
				: Message::SimOutbox);

	foreach (Message message, this->messages) {

		if (false == DeamonSetting::getInstance ()->getMessageStorage ()) {

			DeamonInterface::getInstance ()->write (
						At::writeMessage + QUOTES +
						message.getNumber ()+ QUOTES +
						COMMA + "129,\"STO SENT\"" + NEW_LINE);

			DeamonInterface::getInstance ()->write (
						message.getContent () + CTRL_Z);

			DeamonInterface::getInstance ()->write (
						At::messageCount);
		}

		DeamonInterface::getInstance ()->write (
					At::newMessage + QUOTES + message.getNumber () + QUOTES
					+ COMMA + "129" + NEW_LINE);

		DeamonInterface::getInstance ()->write (message.getContent () + CTRL_Z);
	}

	this->done (MessageDialog::Send);

	return true;
}

bool
MessageDialog::onSaveClicked () {

	this->createMessages (Message::Draft);

	this->done (MessageDialog::Save);

	return true;
}

bool
MessageDialog::onCancelClicked () {

	this->done (MessageDialog::Cancel);

	return true;
}

bool
MessageDialog::createMessages (
		Message::Types type) {

    QStringList toList = this->number->text ().split (SEMI_COLON, QString::SkipEmptyParts);
    QDateTime time = QDateTime::currentDateTime ();
    QStringList contentList;

	QString item = this->content->toPlainText ();

	while (false == item.isEmpty ()) {

		contentList << item.left (MESSAGE_LENGTH);
		item.remove (0, MESSAGE_LENGTH);
	}

	if ( 0 == contentList.count ()) {
		contentList << "";
	}

	foreach (QString to, toList) {
		foreach (QString message, contentList) {
			this->messages << Message( to.trimmed (),
									   "",
									   message.trimmed (),
									   time,
									   type,
									   -1,
									   false);
		}
	}

	return true;
}

QList<Message>
MessageDialog::getMessages () {
	return this->messages;
}

bool
MessageDialog::onModemPlugged () {

	return true;
}

bool
MessageDialog::onModemUnplugged () {

	this->buttons->button (QDialogButtonBox::Yes)->setEnabled (false);

	return true;
}
