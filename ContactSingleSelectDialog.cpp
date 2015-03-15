#include "At.h"
#include "ContactSingleSelectDialog.h"
#include "DeamonDatabase.h"
#include "DeamonInterface.h"
#include "Define.h"
#include "ContactWidget.h"

#include <QPushButton>
#include <QTreeWidgetItem>

ContactSingleSelectDialog::ContactSingleSelectDialog(
                                                    QWidget *parent)
: QDialog(parent)
{

    this->setWindowFlags (Qt::WindowSystemMenuHint|Qt::WindowTitleHint);
    this->setWindowIcon (QIcon(APPLICATION_ICON));

    this->tree = new QTreeWidget;
    this->tree->setHeaderHidden (true);
    this->tree->setColumnCount (1);

    QList<Contact> contacts = DeamonDatabase::getInstance ()->readContact ();

    QTreeWidgetItem * name = NULL;
    QTreeWidgetItem * mobile = NULL;
    QTreeWidgetItem * office = NULL;
    QTreeWidgetItem * home = NULL;

    for (int index = 0; index < contacts.count (); index++)
    {
        name = new QTreeWidgetItem(this->tree);
        name->setText (0, contacts.at (index).getName ());

        if (false == contacts.at (index).getMobile ().isEmpty ())
        {
            mobile =  new QTreeWidgetItem(name);
            mobile->setText (0, contacts.at (index).getMobile ());
        }

        if (false == contacts.at (index).getOffice ().isEmpty ())
        {
            office =  new QTreeWidgetItem(name);
            office->setText (0, contacts.at (index).getOffice());
        }

        if (false == contacts.at (index).getHome ().isEmpty ())
        {
            home =  new QTreeWidgetItem(name);
            home->setText (0, contacts.at (index).getHome ());
        }
    }

    this->buttons = new QDialogButtonBox (
                                         QDialogButtonBox::Ok|
                                         QDialogButtonBox::Cancel);


    this->layout = new QVBoxLayout;
    this->layout->addWidget (this->tree);
    this->layout->addWidget (this->buttons);

    this->setLayout (this->layout);

    QObject::connect (this->tree,
                      SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
                      this,
                      SLOT(onTreeItemDoubleClicked(QTreeWidgetItem*,int)));

    QObject::connect (this->buttons->button (QDialogButtonBox::Ok),
                      SIGNAL(clicked()),
                      this,
                      SLOT(onOkClicked()));

    QObject::connect (this->buttons->button (QDialogButtonBox::Cancel),
                      SIGNAL(clicked()),
                      this,
                      SLOT(onCancelClicked()));

    QObject::connect (DeamonInterface::getInstance (),
                      SIGNAL(readContactUpdate(const Contact&)),
                      this,
                      SLOT(onReadContactUpdate (const Contact&)));

    DeamonInterface::getInstance ()->write (At::readAllContact);
}

bool
ContactSingleSelectDialog::onReadContactUpdate (
                                               const Contact& contact)
{

    QTreeWidgetItem * name = new QTreeWidgetItem(this->tree);
    name->setText (0, contact.getName ());

    if (false == contact.getMobile ().isEmpty ())
    {
        QTreeWidgetItem * mobile =  new QTreeWidgetItem(name);
        mobile->setText (0, contact.getMobile ());
    }

    if (false == contact.getOffice ().isEmpty ())
    {
        QTreeWidgetItem * office =  new QTreeWidgetItem(name);
        office->setText (0, contact.getOffice());
    }

    if (false == contact.getHome ().isEmpty ())
    {
        QTreeWidgetItem * home =  new QTreeWidgetItem(name);
        home->setText (0, contact.getHome ());
    }

    return (true);
}

QString
ContactSingleSelectDialog::getNumber ()
{

    return (this->number);
}

bool
ContactSingleSelectDialog::onTreeItemDoubleClicked (
                                                   QTreeWidgetItem *item,
                                                   int column)
{

    Q_UNUSED (column);

    this->number = item->text (0);
    this->done (QDialog::Accepted);

    return (true);
}

bool
ContactSingleSelectDialog::onOkClicked ()
{

    if ( NULL != this->tree->currentItem ())
    {

        if ( 0 < this->tree->currentItem ()->childCount ())
        {

            this->number = this->tree->currentItem ()->child (0)->text (0);

        }
        else
        {

            this->number = this->tree->currentItem ()->text (0);
        }

    }
    else
    {

        this->number = "";
    }

    this->done (QDialog::Accepted);
    return (true);
}

bool
ContactSingleSelectDialog::onCancelClicked ()
{

    this->number = "";
    this->done (QDialog::Rejected);

    return (true);
}
