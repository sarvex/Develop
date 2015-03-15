#include "ContactSelectDialog.h"
#include "Define.h"

ContactSelectDialog::ContactSelectDialog (
        const Contact& contact,
        QWidget* parent)
    : QDialog (parent) {

    this->initialize (contact.getName (),
                      contact.getMobile (),
                      contact.getOffice (),
                      contact.getHome ());
}

ContactSelectDialog::ContactSelectDialog(
        const QString& name,
        const QString& mobile,
        const QString& office,
        const QString& home,
        int defaultNumber,
        QWidget *parent)
    : QDialog(parent) {

    this->initialize (name,
                      mobile,
                      office,
                      home);

    this->list->setCurrentRow (defaultNumber - 1);
}

bool
ContactSelectDialog::initialize (
        const QString& name,
        const QString& mobile,
        const QString& office,
        const QString& home) {

    this->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
    this->setWindowIcon(QIcon(APPLICATION_ICON));
    this->setWindowTitle (name);

    this->mobile = new QListWidgetItem(mobile);
    this->office = new QListWidgetItem(office);
    this->home = new QListWidgetItem(home);

    this->list = new QListWidget;

    if (false == mobile.isEmpty ()) {

        this->list->addItem (this->mobile);
    }

    if (false == office.isEmpty ()) {

        this->list->addItem (this->office);
    }

    if (false == home.isEmpty ()) {

        this->list->addItem (this->home);
    }

    this->list->setCurrentRow (0);
    this->number = this->list->item (0)->text ();

    this->buttons = new QDialogButtonBox(
                        QDialogButtonBox::Ok|
                        QDialogButtonBox::Cancel);

    this->layout = new QVBoxLayout;
    this->layout->addWidget (this->list);
    this->layout->addWidget (this->buttons);

    this->setLayout (this->layout);

    QObject::connect (this->buttons,
                      SIGNAL(accepted()),
                      this,
                      SLOT(onOkClicked()));

    QObject::connect (this->buttons,
                      SIGNAL(rejected()),
                      this,
                      SLOT(onCancelClicked()));

    QObject::connect (this->list,
                      SIGNAL(itemClicked(QListWidgetItem*)),
                      this,
                      SLOT(onListItemClicked(QListWidgetItem*)));

    QObject::connect (this->list,
                      SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                      this,
                      SLOT(onListItemDoubleClicked(QListWidgetItem*)));

    if (0 == this->list->count ()) {

        this->onCancelClicked ();

    } else if (1 == this->list->count ()) {

        this->number = this->list->item (0)->text ();
        this->onOkClicked ();

    }

    return true;
}

bool
ContactSelectDialog::onListItemClicked (
        QListWidgetItem* item) {

    this->number = item->text ();

    return true;
}

bool
ContactSelectDialog::onListItemDoubleClicked (
        QListWidgetItem* item) {

    this->number = item->text ();

    this->onOkClicked ();
    return true;
}

bool
ContactSelectDialog::onOkClicked () {

    this->done (QDialog::Accepted);

    return true;
}

bool
ContactSelectDialog::onCancelClicked () {

    this->done (QDialog::Rejected);

    return true;
}

QString
ContactSelectDialog::getNumber () {

    return this->number;
}
