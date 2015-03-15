#include "SettingWidget.h"

SettingWidget::SettingWidget (
		QWidget *parent)
	: QWidget(parent) {

	this->list = new QListWidget;

	this->list->addItem (new QListWidgetItem (
							 QIcon(":/local.png"),"Call History"));
	this->list->addItem (new QListWidgetItem (
							 QIcon(":/local.png"),"Data Usage"));
	this->list->addItem (new QListWidgetItem (
							 QIcon(":/local.png"),"Check Balance"));
	this->list->addItem (new QListWidgetItem (
							 QIcon(":/local.png"),"SMS Settings"));
	this->list->addItem (new QListWidgetItem (
							 QIcon(":/local.png"),"Network Settings"));
	this->list->addItem (new QListWidgetItem (
							 QIcon(":/local.png"),"Operators/Product Informations"));
	this->list->addItem (new QListWidgetItem (
							 QIcon(":/local.png"),"PIN Operations"));
	this->list->addItem (new QListWidgetItem (
							 QIcon(":/local.png"),"About PocketInternet SUPER PRO 3G"));
	this->list->addItem (new QListWidgetItem (
							 QIcon(":/local.png"),"User Manual"));

	this->list->item (0)->setToolTip ("Call History");
	this->list->item (1)->setToolTip ("Data Usage");
	this->list->item (2)->setToolTip ("Check Balance");
	this->list->item (3)->setToolTip ("SMS Settings");
	this->list->item (4)->setToolTip ("Network Settings");
	this->list->item (5)->setToolTip ("Operators/Product Informations");
	this->list->item (6)->setToolTip ("PIN Operations");
	this->list->item (7)->setToolTip ("About PocketInternet SUPER PRO 3G");
	this->list->item (8)->setToolTip ("User Manual");

	this->callWidget = new CallWidget;
	this->dataWidget = new DataWidget;
	this->checkBalanceWidget = new CheckBalanceWidget;
	this->messageSettingWidget = new MessageSettingWidget;
	this->networkWidget = new NetworkWidget;
	this->operatorWidget = new OperatorWidget;
	this->pinWidget = new PinWidget;
	this->pocketWidget = new PocketWidget;
	this->userManualWidget = new UserManualWidget;

	this->content = new QStackedWidget;
	this->content->addWidget (this->callWidget);
	this->content->addWidget (this->dataWidget);
	this->content->addWidget (this->checkBalanceWidget);
	this->content->addWidget (this->messageSettingWidget);
	this->content->addWidget (this->networkWidget);
	this->content->addWidget (this->operatorWidget);
	this->content->addWidget (this->pinWidget);
	this->content->addWidget (this->pocketWidget);
	this->content->addWidget (this->userManualWidget);

	this->list->setCurrentRow (0);

	this->splitter = new QSplitter (Qt::Horizontal);
	this->splitter->setChildrenCollapsible (false);
	this->splitter->addWidget (this->list);
	this->splitter->addWidget (this->content);
	this->splitter->setStretchFactor (1, 1);
	this->splitter->setStretchFactor (2, 1);

	this->splitter->setSizes(QList<int> () << 150 << 600);

	this->layout = new QVBoxLayout;
	this->layout->setContentsMargins (0, 0, 0, 0);
	this->layout->addWidget (this->splitter);

	this->setLayout (this->layout);

	QObject::connect (this->list,
					  SIGNAL(currentRowChanged(int)),
					  this->content,
					  SLOT(setCurrentIndex(int)));

	QObject::connect (this,
					  SIGNAL(selectUnread()),
					  this,
					  SLOT(onSelectUnread()));

	QObject::connect (this,
					  SIGNAL(dialed(Call)),
					  this->callWidget,
					  SLOT(add (Call)));

	QObject::connect (this,
					  SIGNAL(answered(Call)),
					  this->callWidget,
					  SLOT(add (Call)));

	QObject::connect (this,
					  SIGNAL(missed(Call)),
					  this->callWidget,
					  SLOT(add(Call)));

	QObject::connect (this,
					  SIGNAL(name (const QString&, const QString&)),
					  this->callWidget,
					  SLOT(name(const QString&, const QString&)));


	QObject::connect (this->callWidget,
					  SIGNAL(dial(const QString&, bool)),
					  this,
					  SIGNAL(dial(const QString&, bool)));

	QObject::connect (this->callWidget,
					  SIGNAL(send(const QString&, const QString&)),
					  this,
					  SIGNAL(send(const QString&, const QString&)));

	QObject::connect (this->callWidget,
					  SIGNAL(save(const QString&)),
					  this,
					  SIGNAL(save(const QString&)));

	QObject::connect (this->callWidget,
					  SIGNAL(number(const QString&)),
					  this,
					  SIGNAL(number(const QString&)));

	QObject::connect (this->callWidget,
					  SIGNAL(callAnnunciator(bool)),
					  this,
					  SIGNAL(callAnnunciator(bool)));

	QObject::connect (this->networkWidget,
					  SIGNAL (resetNetwork ()),
					  this,
					  SIGNAL (resetNetwork ()));
}

bool
SettingWidget::updateTree () {

	this->callWidget->updateTree ();

	return true;
}

bool
SettingWidget::onSelectUnread () {

	this->callWidget->onSelectUnread ();
	this->list->setCurrentRow (0);

	return true;
}
