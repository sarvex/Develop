#include "UserManualWidget.h"

#include <QFile>
#include <QIODevice>
#include <QPainter>
#include <QTextStream>
#include <QUrl>

UserManualWidget::UserManualWidget(
		QWidget *parent)
	: QWidget(parent) {

	this->list = new QListWidget;

	this->list->addItem ("Copyright");
	this->list->addItem ("Getting Started");
	this->list->addItem ("Internet Service");
	this->list->addItem ("Messaging");
	this->list->addItem ("Phonebook");
	this->list->addItem ("Games and Music");
	this->list->addItem ("Voice Call");
	this->list->addItem ("Call History");
	this->list->addItem ("Data Usage");
	this->list->addItem ("Check Balance");
	this->list->addItem ("Online Upgrade");
	this->list->addItem ("Information");
	this->list->addItem ("Security Settings");
	this->list->addItem ("FAQs");

	this->list->item (0)->setToolTip ("Copyright");
	this->list->item (1)->setToolTip ("Getting Started");
	this->list->item (2)->setToolTip ("Internet Service");
	this->list->item (3)->setToolTip ("Messaging");
	this->list->item (4)->setToolTip ("Phonebook");
	this->list->item (5)->setToolTip ("Entertainment");
	this->list->item (6)->setToolTip ("Voice Call");
	this->list->item (7)->setToolTip ("Call History");
	this->list->item (8)->setToolTip ("Data Usage");
	this->list->item (9)->setToolTip ("Check Balance");
	this->list->item (10)->setToolTip ("Online Upgrade");
	this->list->item (11)->setToolTip ("Information");
	this->list->item (12)->setToolTip ("Security Settings");
	this->list->item (13)->setToolTip ("FAQs");

	this->loadManual();

	this->content = new QTextEdit(this);
	this->content->setReadOnly (true);
	this->content->setHtml (this->manual.at(0));

	this->splitter = new QSplitter (Qt::Horizontal, this);
	this->splitter->setChildrenCollapsible (false);

	this->splitter->addWidget (this->list);
	this->splitter->addWidget (this->content);

	this->splitter->setStretchFactor (1, 1);
	this->splitter->setStretchFactor (2, 1);

	this->splitter->setSizes(QList <int> () << 150 << 450);

	this->layout = new QVBoxLayout;
	this->layout->setContentsMargins (0, 0, 0, 0);
	this->layout->addWidget (this->splitter);

	this->setLayout (this->layout);

	QObject::connect (this->list,
					  SIGNAL(currentRowChanged(int)),
					  this,
					  SLOT(onCurrentRowChanged(int)));
}

bool
UserManualWidget::loadManual () {

	QFile file("manual/copyright.html");
	file.open(QIODevice::ReadOnly);
	QTextStream copyrightTextStream(&file);
	this->manual << copyrightTextStream.readAll ();
	file.close();

	file.setFileName ("manual/started.html");
	file.open(QIODevice::ReadOnly);
	QTextStream startedTextStream(&file);
	this->manual << startedTextStream.readAll ();
	file.close();

	file.setFileName ("manual/service.html");
	file.open(QIODevice::ReadOnly);
	QTextStream serviceTextStream(&file);
	this->manual << serviceTextStream.readAll ();
	file.close();

	file.setFileName ("manual/messaging.html");
	file.open(QIODevice::ReadOnly);
	QTextStream messagingTextStream(&file);
	this->manual << messagingTextStream.readAll ();
	file.close();

	file.setFileName ("manual/phonebook.html");
	file.open(QIODevice::ReadOnly);
	QTextStream phonebookTextStream(&file);
	this->manual << phonebookTextStream.readAll ();
	file.close();

	file.setFileName ("manual/entertainment.html");
	file.open(QIODevice::ReadOnly);
	QTextStream entertainmentTextStream(&file);
	this->manual << entertainmentTextStream.readAll ();
	file.close();

	file.setFileName ("manual/call.html");
	file.open(QIODevice::ReadOnly);
	QTextStream callTextStream(&file);
	this->manual << callTextStream.readAll ();
	file.close();

	file.setFileName ("manual/history.html");
	file.open(QIODevice::ReadOnly);
	QTextStream historyTextStream(&file);
	this->manual << historyTextStream.readAll ();
	file.close();

	file.setFileName ("manual/usage.html");
	file.open(QIODevice::ReadOnly);
	QTextStream usageTextStream(&file);
	this->manual << usageTextStream.readAll ();
	file.close();

	file.setFileName ("manual/balance.html");
	file.open(QIODevice::ReadOnly);
	QTextStream balanceTextStream(&file);
	this->manual << balanceTextStream.readAll ();
	file.close();

	file.setFileName ("manual/upgrade.html");
	file.open(QIODevice::ReadOnly);
	QTextStream upgradeTextStream(&file);
	this->manual << upgradeTextStream.readAll ();
	file.close();

	file.setFileName ("manual/information.html");
	file.open(QIODevice::ReadOnly);
	QTextStream informationTextStream(&file);
	this->manual << informationTextStream.readAll ();
	file.close();

	file.setFileName ("manual/security.html");
	file.open(QIODevice::ReadOnly);
	QTextStream securityTextStream(&file);
	this->manual << securityTextStream.readAll ();
	file.close();

	file.setFileName ("manual/faq.html");
	file.open(QIODevice::ReadOnly);
	QTextStream faqTextStream(&file);
	this->manual << faqTextStream.readAll ();
	file.close();

	return true;
}

bool
UserManualWidget::onCurrentRowChanged (
		int index) {

	this->content->setHtml (this->manual.at(index));

	return true;
}
