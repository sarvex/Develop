#include "DataWidget.h"
#include "DeamonConnection.h"
#include "DeamonDatabase.h"
#include "DeamonInterface.h"

#include "Define.h"

DataWidget::DataWidget (
		QWidget *parent)
	: QLabel(parent) {

	Data data = DeamonDatabase::getInstance ()->checkData ();

	this->upload = data.getUpload ();
	this->download = data.getDownload ();


	this->uploadData = new QLineEdit;
	this->uploadData->setText (QString::number (upload, 'f', 4) + " MB");
	this->uploadData->setReadOnly (true);

	this->downloadData = new QLineEdit;
	this->downloadData->setText (QString::number (download, 'f', 4) + " MB");
	this->downloadData->setReadOnly (true);

	this->formLayout = new QFormLayout;
	this->formLayout->setLabelAlignment (Qt::AlignRight);
	this->formLayout->setContentsMargins (10,25,10,0);
	this->formLayout->addRow ("Uploaded", this->uploadData);
	this->formLayout->addRow ("Downloaded", this->downloadData);

	this->groupBox = new QGroupBox("Data Usage Details");
	this->groupBox->setLayout (this->formLayout);

	this->buttons = new QDialogButtonBox(QDialogButtonBox::Reset);

	this->note = new QLabel(DATA_USAGE_NOTE);
	this->note->setWordWrap (true);

	this->layout = new QVBoxLayout;
	this->layout->setContentsMargins (0, 0, 0, 0);
	this->layout->addWidget (this->groupBox);
	this->layout->addStretch ();
	this->layout->addWidget (this->note);
	this->layout->addWidget (this->buttons);

	this->setLayout (this->layout);

	QObject::connect (this->buttons->button (QDialogButtonBox::Reset),
					  SIGNAL(clicked()),
					  this,
					  SLOT(onResetClicked()));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(modemPlugged (bool)),
					  this,
					  SLOT(onModemPlugged (bool)));

	QObject::connect (DeamonConnection::getInstance (),
					  SIGNAL(modemConnected (bool)),
					  this,
					  SLOT(onModemConnected (bool)));

	QObject::connect (DeamonConnection::getInstance (),
					  SIGNAL (usageUpdate (double, double)),
					  this,
					  SLOT(onUsageUpdate(double,double)));
}

void
DataWidget::showEvent (
		QShowEvent * event) {


	QWidget::showEvent (event);
}


bool
DataWidget::onResetClicked(){

	DeamonDatabase::getInstance ()->removeData ();
	this->uploadData->setText ("0.0000 MB");
	this->downloadData->setText ("0.0000 MB");

	this->upload = 0.0;
	this->download = 0.0;

	emit resetUsage ();

	return true;
}

bool
DataWidget::onUsageUpdate (
		double upload,
		double download) {

	this->upload += (upload / 1024);
	this->download += (download / 1024);

	this->uploadData->setText (QString::number (this->upload, 'f', 4) + " MB");
	this->downloadData->setText (QString::number (this->download, 'f', 4) + " MB");

	return true;
}

bool
DataWidget::onModemPlugged (
		bool status) {
	Q_UNUSED (status);

	return true;
}

bool
DataWidget::onModemConnected (
		bool status) {
	Q_UNUSED (status);

	return true;
}
