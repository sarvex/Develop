#include "At.h"
#include "DeamonConnection.h"
#include "DeamonInterface.h"
#include "Define.h"
#include "NetworkSelectionWidget.h"

#include <QHeaderView>

NetworkSelectionWidget::NetworkSelectionWidget (
		QWidget *parent)
	: QWidget(parent){

	this->header << "Status"
				 << "Network"
				 << "Operator"
				 << "Identifier"
				 << "Mode" << "";

	this->mode << ""
			   << "Available"
			   << "Registered"
			   << "Forbidden";

	this->status << "2G"
				 << "2G"
				 << "3G";

	this->automaticButton = new QRadioButton("Automatic");
	this->automaticButton->setChecked(true);

	this->manualButton = new QRadioButton("Manual");
	this->manualButton->setChecked (false);

	this->refreshButton = new QPushButton("Refresh");
	this->refreshButton->setEnabled (false);

	this->registerButton = new QPushButton("Register");
	this->registerButton->setEnabled (false);

	this->progressBar = new QProgressBar (this);
	this->progressBar->setMinimum (0);
	this->progressBar->setMaximum (0);
	this->progressBar->setVisible (false);

	this->table = new QTableWidget (this);
	this->table->setSelectionBehavior (QAbstractItemView::SelectRows);
	this->table->setColumnCount (6);
	this->table->setColumnHidden (5, true);
	this->table->verticalHeader ()->setDefaultSectionSize (18);
	this->table->verticalHeader ()->setVisible (false);
	this->table->horizontalHeader ()->setStretchLastSection (true);
	this->table->setHorizontalHeaderLabels (this->header);
	this->table->setSortingEnabled (true);
	this->table->setVisible (false);

	this->layout = new QGridLayout;
	this->layout->addWidget (this->automaticButton, 0, 0, 1, 1);
	this->layout->addWidget (this->manualButton, 0, 1, 1, 1);
	this->layout->addWidget (this->refreshButton, 0, 4, 1, 1);
	this->layout->addWidget (this->registerButton, 0, 5, 1, 1);
	this->layout->addWidget (this->progressBar, 1, 0, 1, 6);
	this->layout->addWidget (this->table, 1, 0, 8, 6);
	this->setLayout (this->layout);

	QObject::connect (this->automaticButton,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onAutomaticButtonClicked()));

	QObject::connect (this->manualButton,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onManualButtonClicked()));

	QObject::connect (this->refreshButton,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onRefreshButtonClicked()));


	QObject::connect (this->registerButton,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onRegisterButtonClicked()));

	QObject::connect (this->table,
					  SIGNAL(itemClicked(QTableWidgetItem*)),
					  this,
					  SLOT(onTableItemClicked(QTableWidgetItem*)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(networkModeUpdate (const QString&,
												const QString&,
												const QString&,
												const QString&,
												const QString&)),
					  this,
					  SLOT(onNetworkModeUpdate (const QString&,
												const QString&,
												const QString&,
												const QString&,
												const QString&)));

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
NetworkSelectionWidget::showEvent (
	QShowEvent * event) {

	if ( (true == this->manualButton->isChecked ()) &&
		 (true == this->manualButton->isEnabled ()) ) {

		this->updateNetworkMode ();
	}

	QWidget::showEvent (event);
}

bool
NetworkSelectionWidget::onTableItemClicked (
		QTableWidgetItem* item) {

	if (NULL == item) {

		this->registerButton->setEnabled (false);
		return false;
	}

	int row = item->row ();

	if (this->table->item (row, 0)->text ().contains (this->mode.at (1))) {

		this->registerButton->setEnabled (true);

	} else {

		this->registerButton->setEnabled (false);
	}

	return true;
}

bool
NetworkSelectionWidget::updateNetworkMode () {

	this->refreshButton->setEnabled (false);
	this->registerButton->setEnabled (false);

	this->progressBar->setVisible (true);

	this->table->clear ();
	this->table->setRowCount (0);
	this->table->setHorizontalHeaderLabels (this->header);
	this->table->setVisible (false);

	DeamonInterface::getInstance ()->write (At::networkMode);

	return true;
}

bool
NetworkSelectionWidget::onNetworkModeUpdate (
		QString mode,
		QString major,
		QString minor,
		QString identifier,
		QString status) {

	this->progressBar->setVisible (false);
	this->table->setVisible (true);

	this->refreshButton->setEnabled (false);

	int index = this->table->rowCount ();
	this->table->insertRow (index);

	QTableWidgetItem * data = NULL;

	data = new QTableWidgetItem(this->mode.at (mode.toInt ()));
	data->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	this->table->setItem (index, 0, data);

	data = new QTableWidgetItem(major);
	data->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	this->table->setItem (index, 1, data);

	data = new QTableWidgetItem(minor);
	data->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	this->table->setItem (index, 2, data);

	data = new QTableWidgetItem(identifier);
	data->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	this->table->setItem (index, 3, data);

	data = new QTableWidgetItem(this->status.at (status.toInt ()));
	data->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	this->table->setItem (index, 4, data);

	data = new QTableWidgetItem(status);
	data->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	this->table->setItem (index, 5, data);

	this->refreshButton->setEnabled (true);
	this->manualButton->setChecked (true);

	return true;
}

bool
NetworkSelectionWidget::onRefreshButtonClicked () {

	this->updateNetworkMode ();

	return true;
}

bool
NetworkSelectionWidget::onRegisterButtonClicked () {

	int row = this->table->currentRow ();

	if (0 > row) {
		QMessageBox::warning (NULL,
							  NO_SELECTION,
							  NO_SELECTION_NETWORK,
							  QMessageBox::Ok,
							  QMessageBox::Ok);

		return false;
	}

	DeamonInterface::getInstance ()->write (
				At::manualNetworkSelection + QUOTES +
				this->table->item (row, 2)->text () + QUOTES + COMMA +
				this->table->item (row, 5)->text ());

	QMessageBox::information (NULL,
							  "Network Selection",
							  "Trying to register manually on network.",
							  QMessageBox::Ok,
							  QMessageBox::Ok);

	this->table->setVisible (false);
	this->progressBar->setVisible (true);

	QTimer::singleShot (15000, this, SLOT(onManualButtonClicked()));

	return true;
}

bool
NetworkSelectionWidget::onAutomaticButtonClicked () {

	this->refreshButton->setEnabled (false);
	this->registerButton->setEnabled (false);

	this->table->setVisible (false);
	this->progressBar->setVisible (false);

	DeamonInterface::getInstance ()->write (At::automaticNetworkSelection);

	QMessageBox::information (NULL,
							  "Network Selection",
							  "Trying to register automatically on network.",
							  QMessageBox::Ok,
							  QMessageBox::Ok);

	return true;
}

bool
NetworkSelectionWidget::onManualButtonClicked () {

	this->updateNetworkMode ();

	return true;
}

bool
NetworkSelectionWidget::onModemPlugged (bool status) {

	return true;
}

bool
NetworkSelectionWidget::onModemConnected (bool status) {

	if (true == status) {

		this->automaticButton->setEnabled (false);
		this->manualButton->setEnabled (false);

		this->refreshButton->setEnabled (false);

	} else {

		this->automaticButton->setEnabled (true);
		this->manualButton->setEnabled (true);
	}
	return true;
}
