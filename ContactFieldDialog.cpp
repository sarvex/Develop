#include "ContactFieldDialog.h"
#include "Define.h"

ContactFieldDialog::ContactFieldDialog (
        QWidget *parent)
: QDialog(parent)
{

    this->setWindowFlags (Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
    this->setWindowIcon (QIcon(APPLICATION_ICON));
    this->setWindowTitle (tr("Select Fields to Synchronize"));

    this->list = new QListWidget;

    this->name = new QCheckBox(tr("Name"));
    this->name->setChecked (true);
    this->nameItem = new QListWidgetItem(this->list);
    this->list->setItemWidget (this->nameItem, this->name);

    this->mobile = new QCheckBox(tr("Mobile"));
    this->mobile->setChecked (true);
    this->mobileItem = new QListWidgetItem(this->list);
    this->list->setItemWidget (this->mobileItem, this->mobile);

    this->office = new QCheckBox(tr("Office"));
    this->office->setChecked (true);
    this->officeItem = new QListWidgetItem(this->list);
    this->list->setItemWidget (this->officeItem, this->office);

    this->home = new QCheckBox(tr("Home"));
    this->home->setChecked (true);
    this->homeItem = new QListWidgetItem(this->list);
    this->list->setItemWidget (this->homeItem, this->home);

    this->email = new QCheckBox(tr("Email"));
    this->email->setChecked (true);
    this->emailItem = new QListWidgetItem(this->list);
    this->list->setItemWidget (this->emailItem, this->email);

    this->remark = new QCheckBox(tr("Remark"));
    this->remark->setChecked (true);
    this->remarkItem = new QListWidgetItem(this->list);
    this->list->setItemWidget (this->remarkItem, this->remark);

    this->type = new QCheckBox(tr("Type"));
    this->type->setChecked (true);
    this->typeItem = new QListWidgetItem(this->list);
    this->list->setItemWidget (this->typeItem, this->type);

    this->buttons = new QDialogButtonBox (
                        QDialogButtonBox::Ok
                        |QDialogButtonBox::Cancel);

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
ContactFieldDialog::onOkClicked ()
{

    this->done (QDialog::Accepted);

    return (true);
}

bool
ContactFieldDialog::onCancelClicked ()
{

    this->done (QDialog::Rejected);

    return (true);
}

QList<bool>
ContactFieldDialog::getFields ()
{

    QList<bool> result;

    for (int index = 0; index < this->list->count (); index++)
    {

        result << dynamic_cast<QCheckBox*> (
                      this->list->itemWidget (
                          this->list->item (index)))->isChecked ();
    }

    return (result);
}
