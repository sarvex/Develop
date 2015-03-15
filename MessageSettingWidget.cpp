#include "At.h"
#include "DeamonConnection.h"
#include "DeamonInterface.h"
#include "DeamonSetting.h"
#include "Define.h"
#include "MessageSettingWidget.h"

#include <QMessageBox>

MessageSettingWidget::MessageSettingWidget(QWidget *parent) : QLabel(parent) {
	this->saveLocalRadioButton = new QRadioButton("Save in local");
	this->saveLocalRadioButton->setChecked (DeamonSetting::getInstance ()->getMessageStorage ());
	QObject::connect (this->saveLocalRadioButton, SIGNAL(clicked()), SLOT(onChanged()));

	this->saveSimRadioButton = new QRadioButton("Save on SIM/USIM card or device");
	this->saveSimRadioButton->setChecked (!DeamonSetting::getInstance ()->getMessageStorage ());
	QObject::connect (this->saveSimRadioButton, SIGNAL(clicked()), SLOT(onChanged()));

	this->groupLayout = new QVBoxLayout;
	this->groupLayout->addWidget (this->saveLocalRadioButton);
	this->groupLayout->addWidget (this->saveSimRadioButton);

	this->groupBox = new QGroupBox("New Message Save Mode");
	this->groupBox->setLayout (this->groupLayout);

	this->delieveryReport = new QCheckBox ("Enable Delivery Report");
	this->delieveryReport->setChecked (DeamonSetting::getInstance ()->getDeliveryReport ());
	QObject::connect (this->delieveryReport, SIGNAL(stateChanged(int)), SLOT(onDeliveryReport(int)));

	this->buttons = new QDialogButtonBox (QDialogButtonBox::Apply);
	this->buttons->button (QDialogButtonBox::Apply)->setEnabled (false);
	QObject::connect (this->buttons->button (QDialogButtonBox::Apply), SIGNAL(clicked()), SLOT(onApplyClicked()));

	this->layout = new QVBoxLayout;
	this->layout->setContentsMargins (0, 0, 0, 0);
	this->layout->addWidget (this->groupBox);
	this->layout->addWidget (this->delieveryReport);
	this->layout->addStretch ();
	this->layout->addWidget (this->buttons);
	this->setLayout (this->layout);

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(modemPlugged (bool)),
					  this,
					  SLOT(onModemPlugged (bool)));

	QObject::connect (DeamonConnection::getInstance (),
					  SIGNAL(modemConnected (bool)),
					  this,
					  SLOT(onModemConnected (bool)));}

bool MessageSettingWidget::onChanged () {
	this->buttons->button (QDialogButtonBox::Apply)->setEnabled (true);

	return false;
}

bool MessageSettingWidget::onApplyClicked () {

	DeamonSetting::getInstance ()->setDeliveryReport (true == this->delieveryReport->isChecked ());

	QMessageBox::information(NULL, "Message Settings", "Message setting saved", QMessageBox::Ok, QMessageBox::Ok);

	this->buttons->button (QDialogButtonBox::Apply)->setEnabled (false);
	return true;
}

bool
MessageSettingWidget::onModemPlugged (
		bool status) {

	this->delieveryReport->setVisible (status);

	return true;
}

bool
MessageSettingWidget::onModemConnected (
		bool status) {
	Q_UNUSED (status);

	return true;
}

bool MessageSettingWidget::onDeliveryReport (int state) {
	Q_UNUSED (state);
	this->buttons->button (QDialogButtonBox::Apply)->setEnabled (true);

	return true;
}
