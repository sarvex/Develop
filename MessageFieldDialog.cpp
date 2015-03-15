#include "MessageFieldDialog.h"
#include "Define.h"

MessageFieldDialog::MessageFieldDialog (
		QWidget *parent)
	: QDialog(parent) {

	this->setWindowFlags (Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
	this->setWindowIcon (QIcon(APPLICATION_ICON));
	this->setWindowTitle ("Select the Message Fields to Synchronize");

	this->list = new QListWidget;

	this->number = new QCheckBox("Number");
	this->number->setChecked (true);
	this->numberItem = new QListWidgetItem(this->list);
	this->list->setItemWidget (this->numberItem, this->number);

	this->name = new QCheckBox("Name");
	this->name->setChecked (true);
	this->nameItem = new QListWidgetItem(this->list);
	this->list->setItemWidget (this->nameItem, this->name);

	this->content = new QCheckBox("Content");
	this->content->setChecked (true);
	this->contentItem = new QListWidgetItem(this->list);
	this->list->setItemWidget (this->contentItem, this->content);

	this->time = new QCheckBox("Time");
	this->time->setChecked (true);
	this->timeItem = new QListWidgetItem(this->list);
	this->list->setItemWidget (this->timeItem, this->time);

	this->type = new QCheckBox("Type");
	this->type->setChecked (true);
	this->typeItem = new QListWidgetItem(this->list);
	this->list->setItemWidget (this->typeItem, this->type);

	this->buttons = new QDialogButtonBox(
						QDialogButtonBox::Ok|
						QDialogButtonBox::Cancel);

	QObject::connect (this->buttons,
					  SIGNAL(accepted()),
					  this,
					  SLOT(onOkClicked()));

	QObject::connect (this->buttons,
					  SIGNAL(rejected()),
					  this,
					  SLOT(onCancelClicked()));

	this->layout = new QVBoxLayout;
	this->layout->addWidget (this->list);
	this->layout->addWidget (this->buttons);

	this->setLayout (this->layout);
}

bool
MessageFieldDialog::onOkClicked () {
	this->done (QDialog::Accepted);

	return true;
}

bool
MessageFieldDialog::onCancelClicked () {
	this->done (QDialog::Rejected);

	return true;
}

QList<bool>
MessageFieldDialog::getFields () {

	QList<bool> result;

	for (int index = 0; index < list->count (); index++) {

		result << dynamic_cast<QCheckBox*> (
					  this->list->itemWidget (
						  this->list->item (index)))->isChecked ();
	}

	return result;
}
