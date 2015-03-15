#include "At.h"
#include "ContactMultipleSelectDialog.h"
#include "DeamonDatabase.h"
#include "DeamonInterface.h"
#include "Define.h"
#include "ContactWidget.h"

ContactMultipleSelectDialog::ContactMultipleSelectDialog (
                                                         QWidget *parent)
: QDialog(parent)
{

    this->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
    this->setWindowIcon(QIcon(APPLICATION_ICON));

    this->createTree ();
    this->createList ();
    this->createLayout ();

    DeamonInterface::getInstance ()->write (At::readAllContact);

    QObject::connect (DeamonInterface::getInstance (),
                      SIGNAL(readContactUpdate(const Contact&)),
                      this,
                      SLOT(onReadContactUpdate (const Contact&)));
}

bool
ContactMultipleSelectDialog::createTree ()
{

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

    QObject::connect (this->tree,
                      SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
                      this,
                      SLOT(onTreeItemDoubleClicked(QTreeWidgetItem*,int)));

    return (true);
}

bool
ContactMultipleSelectDialog::onReadContactUpdate (
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

bool
ContactMultipleSelectDialog::createList ()
{
    this->list = new QListWidget;

    QObject::connect (this->list,
                      SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                      this,
                      SLOT(onListItemDoubleClicked(QListWidgetItem*)));
    return (true);
}

bool
ContactMultipleSelectDialog::createLayout ()
{

    this->splitter = new QSplitter(Qt::Horizontal);
    this->splitter->addWidget (this->tree);
    this->splitter->addWidget (this->list);

    this->addContactHelp = new QLabel (ADD_CONTACT_HELP);
    this->addContactHelp->setWordWrap (true);

    this->removeContactHelp = new QLabel ();
    this->removeContactHelp->setWordWrap (true);

    this->helpLayout = new QHBoxLayout;
    this->helpLayout->addWidget (this->addContactHelp);
    this->helpLayout->addWidget (this->removeContactHelp);

    this->buttons = new QDialogButtonBox (
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
    this->layout->addWidget (this->splitter);
    this->layout->addLayout (this->helpLayout);
    this->layout->addWidget (this->buttons);
    this->setLayout (this->layout);

    return (true);
}

bool
ContactMultipleSelectDialog::onOkClicked ()
{

    this->done (QDialog::Accepted);

    return (true);
}

bool
ContactMultipleSelectDialog::onCancelClicked ()
{

    this->done (QDialog::Rejected);

    return (true);
}

bool
ContactMultipleSelectDialog::onTreeItemDoubleClicked (
                                                     QTreeWidgetItem* item,
                                                     int row)
{

    Q_UNUSED (row);

    if ( 0 < item->childCount () )
    {

        for (int index = 0; index < item->childCount (); index++)
        {

            this->list->addItem (
                                new QListWidgetItem(item->child (index)->text (0)));
        }

    }
    else
    {

        this->list->addItem (new QListWidgetItem(item->text (0)));

    }

    return (true);
}

bool
ContactMultipleSelectDialog::onListItemDoubleClicked (
                                                     QListWidgetItem *item)
{

    this->list->removeItemWidget (item);
    delete item;

    return (true);
}

QStringList
ContactMultipleSelectDialog::getNumbers ()
{

    QStringList result;

    for (int index = 0; index < this->list->count (); index++)
    {

        result << this->list->item (index)->text ();
    }

    return (result);
}
