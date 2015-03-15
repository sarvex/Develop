#include "At.h"
#include "ContactSelectDialog.h"
#include "ContactWidget.h"
#include "ContactDialog.h"
#include "ContactFieldDialog.h"
#include "DeamonConnection.h"
#include "DeamonDatabase.h"
#include "DeamonInterface.h"
#include "Define.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QHeaderView>
#include <QIcon>
#include <QMessageBox>
#include <QString>
#include <QVariant>

ContactWidget::ContactWidget(QWidget *parent)
: QWidget(parent)
{

    this->initialize ();

    this->createActions ();
    this->createToolBar ();
    this->createContextMenu ();

    this->createTree ();
    this->createTable ();

    this->createLayout ();
    this->connect ();
}

/// Actions
bool
ContactWidget::createActions ()
{

    for (int index = 0; index < Action::Count; index++)
    {

        this->action << new QAction (
                                    this->actionIcon.at (index),
                                    this->actionName.at (index),
                                    this);

        this->action[index]->setShortcut (this->actionShortcut.at (index));
        this->action[index]->setToolTip (this->actionToolTip.at (index));
    }

    for (int index = 0; index < Separator::Count; index++)
    {

        this->separator << new QAction (this);
        this->separator[index]->setSeparator (true);
    }

    QObject::connect (this->action.at (Action::New),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onNewTriggered()));

    QObject::connect (this->action.at (Action::Edit),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onEditTriggered()));

    QObject::connect (this->action.at (Action::Delete),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onDeleteTriggered()));

    QObject::connect (this->action.at (Action::Call),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onCallTriggered()));

    QObject::connect (this->action.at (Action::Message),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onMessageTriggered()));

    QObject::connect (this->action.at (Action::Send),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onSendTriggered()));

    QObject::connect (this->action.at (Action::Find),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onFindTriggered()));

    QObject::connect (this->action.at (Action::Import),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onImportTriggered()));

    QObject::connect (this->action.at (Action::Export),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onExportTriggered()));

    QObject::connect (this->action.at (Action::MoveToApplication),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onMoveToApplicationTriggered()));

    QObject::connect (this->action.at (Action::MoveToSim),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onMoveToSimTriggered()));

    QObject::connect (this->action.at (Action::CopyToApplication),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onCopyToApplicationTriggered()));

    QObject::connect (this->action.at (Action::CopyToSim),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onCopyToSimTriggered()));

    return (true);
}

bool
ContactWidget::onNewTriggered ()
{

    Contact contact;

    QTreeWidgetItem * item = this->tree->currentItem ();

    if ((NULL != item) &&
        (item->statusTip (0).contains (
                                      QString::number (static_cast<int>(Contact::Sim)))))
    {

        contact.setType (Contact::Sim);

    }
    else if ( (NULL != item) &&
              (item->statusTip (0).contains (
                                            QString::number (static_cast <int> (Contact::Other)))) )
    {
        contact.setType (Contact::Other);

    }
    else if ( (NULL != item) &&
              (item->statusTip (0).contains (
                                            QString::number (static_cast <int> (Contact::Family)))) )
    {
        contact.setType (Contact::Family);

    }
    else if ( (NULL != item) &&
              (item->statusTip (0).contains (
                                            QString::number (static_cast <int> (Contact::Friend)))) )
    {
        contact.setType (Contact::Friend);

    }
    else if ( (NULL != item) &&
              (item->statusTip (0).contains (
                                            QString::number (static_cast <int> (Contact::Colleague)))) )
    {
        contact.setType (Contact::Colleague);
    }

    ContactDialog dialog(contact);

    if ( QDialog::Accepted == dialog.exec() )
    {

        Contact newContact = dialog.getContact ();

        if (Contact::Sim == contact.getType ())
        {

            DeamonInterface::getInstance ()->write (
                                                   At::writeContact + QUOTES +
                                                   newContact.getMobile () +
                                                   + QUOTES + COMMA + COMMA + QUOTES +
                                                   newContact.getName ()+QUOTES);

        }
        else
        {

            this->addUpdate (newContact);
        }

        return (true);

    }
    else
    {

        return (false);
    }
}

bool
ContactWidget::onEditTriggered ()
{

    int index = this->table->currentRow ();
    if ( 0 > index)
    {

        QMessageBox::warning (NULL,
                              NO_SELECTION,
                              NO_SELECTION_CONTACT,
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return (false);
    }

    ContactDialog contact (this->rowToContact (index));

    if ( QDialog::Accepted == contact.exec() )
    {

        this->table->removeRow (index);
        this->edit (contact.getContact ());
        return (true);

    }
    else
    {

        return (false);
    }
}

bool
ContactWidget::onDeleteTriggered ()
{

    this->table->setSortingEnabled (false);

    QList<int> indices;
    QList<QTableWidgetSelectionRange> ranges = this->table->selectedRanges ();

    foreach (QTableWidgetSelectionRange range, ranges)
    {

        for (int row = range.topRow (); row <= range.bottomRow (); row++)
        {

            if (false == this->table->isRowHidden (row))
            {

                indices << row;
            }
        }
    }

    qSort(indices.begin (), indices.end (), qGreater<int> ());

    foreach (int index, indices)
    {
        this->remove (index);
    }

    this->updateTable ();
    this->updateTree ();

    this->table->setSortingEnabled (true);
    return (true);
}

bool
ContactWidget::onCallTriggered ()
{

    int index = this->table->currentRow ();
    if ( 0 > index)
    {

        QMessageBox::warning (NULL,
                              NO_SELECTION,
                              NO_SELECTION_CONTACT,
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return (false);
    }

    ContactSelectDialog dialog(this->rowToContact (index));

    if (QDialog::Accepted == dialog.exec ())
    {

        emit dial (dialog.getNumber (), this);
        return (true);

    }
    else
    {

        return (false);
    }
}

bool
ContactWidget::onMessageTriggered ()
{

    int index = this->table->currentRow ();
    if ( 0 > index)
    {

        QMessageBox::warning (NULL,
                              NO_SELECTION,
                              NO_SELECTION_CONTACT,
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return (false);
    }

    ContactSelectDialog dialog(this->rowToContact (index));

    if (QDialog::Accepted == dialog.exec ())
    {

        emit send (dialog.getNumber (), "");
        return (true);

    }
    else
    {

        return (false);
    }
}

bool
ContactWidget::onSendTriggered ()
{

    int row = this->table->currentRow ();
    if ( 0 > row)
    {

        QMessageBox::warning (NULL,
                              NO_SELECTION,
                              NO_SELECTION_CONTACT,
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return (false);
    }

    QString content;
    for (int index = 0; index < Contact::FieldCount; index ++)
    {

        if (false == this->table->isColumnHidden (index))
        {

            content += Contact::Header.at (index)
                       + COLON
                       + this->table->item (row, index)->text ()
                       + NEW_LINE_CARRIAGE;
        }
    }

    emit send ("", content);
    return (true);

}

bool
ContactWidget::onFindTriggered ()
{

    if (true == this->searchEditAction->isVisible ())
    {

        this->searchEditAction->setVisible (false);

    }
    else
    {

        this->searchEditAction->setVisible (true);
        this->searchEdit->setFocus ();
    }

    return (true);
}

bool
ContactWidget::onImportTriggered ()
{

    ContactFieldDialog fields;

    if ( QDialog::Rejected == fields.exec ())
    {

        return (false);

    }
    else
    {

        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr ("Export to CSV"),
                                                        QDir::tempPath (),
                                                        tr ("CSV Files (*.csv)"));

        if (fileName.isNull ())
        {

            return (false);

        }
        else
        {

            QFile file(fileName);
            file.open (QIODevice::ReadOnly);

            QTextStream textStream (&file);
            QString line = textStream.readLine ();

            this->table->setSortingEnabled (false);

            while ( false == line.isNull ())
            {

                this->read (fields.getFields (), line);
                line = textStream.readLine ();
            }

            this->updateTable ();
            this->updateTree ();

            this->table->setSortingEnabled (true);
            file.close ();

            return (true);
        }
    }
}

bool
ContactWidget::onExportTriggered ()
{

    int row = this->table->currentRow ();

    if ( 0 > row)
    {

        QMessageBox::warning (NULL,
                              NO_SELECTION,
                              NO_SELECTION_CONTACT,
                              QMessageBox::Ok,
                              QMessageBox::Ok);

        return (false);

    }
    else
    {

        ContactFieldDialog fields;
        if ( QDialog::Rejected == fields.exec ())
        {

            return (false);

        }
        else
        {

            QString fileName =
            QFileDialog::getSaveFileName (this,
                                          tr("Export to CSV"),
                                          QDir::tempPath (),
                                          tr("CSV Files (*.csv)"));

            if (fileName.isNull ())
            {

                return (false);

            }
            else
            {

                QFile file(fileName);
                file.open (QIODevice::WriteOnly | QIODevice::Append);

                QTextStream textStream (&file);

                this->table->setSortingEnabled (false);

                QList<int> indices;

                QList<QTableWidgetSelectionRange>
                ranges = this->table->selectedRanges ();

                foreach (QTableWidgetSelectionRange range, ranges)
                {

                    for (int row = range.topRow ();
                        row <= range.bottomRow (); row++)
                    {

                        if (false == this->table->isRowHidden (row))
                        {

                            indices << row;
                        }
                    }
                }

                qSort(indices.begin (), indices.end (), qGreater<int> ());

                foreach (int index, indices)
                {

                    textStream << this->write (fields.getFields (), index);
                }

                this->table->setSortingEnabled (true);
                file.close ();

                return (true);
            }
        }
    }

}

bool
ContactWidget::onMoveToApplicationTriggered ()
{

    this->table->setSortingEnabled (false);

    QList<int> indices;

    QList<QTableWidgetSelectionRange> ranges = this->table->selectedRanges ();

    foreach (QTableWidgetSelectionRange range, ranges)
    {

        for (int row = range.topRow (); row <= range.bottomRow (); row++)
        {

            if (false == this->table->isRowHidden (row))
            {

                indices << row;
            }
        }
    }

    qSort(indices.begin (), indices.end (), qGreater<int> ());

    foreach (int index, indices)
    {

        this->moveToApplication (index);
    }

    this->updateTree ();
    this->updateTable ();

    this->table->setSortingEnabled (true);

    return (true);
}

bool
ContactWidget::onMoveToSimTriggered ()
{

    this->table->setSortingEnabled (false);

    QList<int> indices;

    QList<QTableWidgetSelectionRange> ranges = this->table->selectedRanges ();

    foreach (QTableWidgetSelectionRange range, ranges)
    {

        for (int row = range.topRow (); row <= range.bottomRow (); row++)
        {

            if (false == this->table->isRowHidden (row))
            {

                indices << row;
            }
        }
    }

    qSort(indices.begin (), indices.end (), qGreater<int> ());

    foreach (int index, indices)
    {

        this->moveToSim (index);
    }

    this->updateTree ();
    this->updateTable ();

    this->table->setSortingEnabled (true);

    return (true);
}

bool
ContactWidget::onCopyToApplicationTriggered ()
{

    this->table->setSortingEnabled (false);

    QList<int> indices;

    QList<QTableWidgetSelectionRange> ranges = this->table->selectedRanges ();

    foreach (QTableWidgetSelectionRange range, ranges)
    {

        for (int row = range.topRow (); row <= range.bottomRow (); row++)
        {

            if (false == this->table->isRowHidden (row))
            {

                indices << row;
            }
        }
    }

    qSort(indices.begin (), indices.end (), qGreater<int> ());

    foreach (int index, indices)
    {

        this->copyToApplication (index);
    }

    this->updateTree ();
    this->updateTable ();

    this->table->setSortingEnabled (true);
    return (true);
}

bool
ContactWidget::onCopyToSimTriggered ()
{

    this->table->setSortingEnabled (false);

    QList<int> indices;

    QList<QTableWidgetSelectionRange> ranges = this->table->selectedRanges ();

    foreach (QTableWidgetSelectionRange range, ranges)
    {

        for (int row = range.topRow (); row <= range.bottomRow (); row++)
        {

            if (false == this->table->isRowHidden (row))
            {

                indices << row;
            }
        }
    }

    qSort(indices.begin (), indices.end (), qGreater<int> ());

    foreach (int index, indices)
    {

        this->copyToSim (index);
    }

    this->updateTree ();
    this->updateTable ();

    this->table->setSortingEnabled (true);
    return (true);
}

bool
ContactWidget::createTree ()
{

    this->tree = new QTreeWidget;

    this->tree->setColumnCount (1);
    this->tree->setHeaderLabel (tr("Contacts"));

    this->item << new QTreeWidgetItem (this->tree);

    this->item << new QTreeWidgetItem (this->item.at (Contact::All));
    this->item << new QTreeWidgetItem (this->item.at (Contact::All));

    this->item << new QTreeWidgetItem (this->item.at (Contact::Local));
    this->item << new QTreeWidgetItem (this->item.at (Contact::Local));
    this->item << new QTreeWidgetItem (this->item.at (Contact::Local));
    this->item << new QTreeWidgetItem (this->item.at (Contact::Local));

    this->item.at (Contact::All)->setExpanded (true);
    this->item.at (Contact::Local)->setExpanded (true);

    this->item.at (Contact::Sim)->setHidden (true);

    for (int index = 0; index < this->item.count (); index++)
    {

        this->item[index]->setText (0, Contact::Display.at (index));
        this->item[index]->setIcon (0, this->treeIcon.at (index));
        this->item[index]->setToolTip (0, Contact::ToolTip.at (index));
        this->item[index]->setStatusTip (0, Contact::String.at (index));
    }

    this->tree->setCurrentItem (this->item.at (Contact::Local));
    this->tree->setColumnWidth (0, 170);

    QObject::connect (this->tree,
                      SIGNAL(currentItemChanged(
                                               QTreeWidgetItem*,
                                               QTreeWidgetItem*)),
                      this,
                      SLOT(onTreeCurrentItemChanged(
                                                   QTreeWidgetItem*,
                                                   QTreeWidgetItem*)));

    return (true);
}

bool
ContactWidget::onTreeCurrentItemChanged(
                                       QTreeWidgetItem* current,
                                       QTreeWidgetItem* previous)
{
    Q_UNUSED (current);
    Q_UNUSED (previous);

    this->updateTable ();
    this->content->clear ();

    return (true);
}

/// Toolbar
bool
ContactWidget::createToolBar ()
{

    this->toolbar = new QToolBar (tr("Contacts"));
    this->toolbar->setFixedHeight (36);

    this->toolbar->addAction (this->action.at (Action::New));
    this->toolbar->addAction (this->action.at (Action::Edit));
    this->toolbar->addAction (this->action.at (Action::Delete));
    this->toolbar->addSeparator ();

    this->toolbar->addAction (this->action.at (Action::Call));
    this->toolbar->addAction (this->action.at (Action::Message));
    this->toolbar->addAction (this->action.at (Action::Send));
    this->toolbar->addSeparator ();

    this->toolbar->addAction (this->action.at (Action::Import));
    this->toolbar->addAction (this->action.at (Action::Export));
    this->toolbar->addSeparator ();

    this->toolbar->addAction (this->action.at (Action::Find));

    this->searchEdit = new QLineEdit (this);
    QObject::connect (this->searchEdit,
                      SIGNAL(textChanged(QString)),
                      this,
                      SLOT(onSearchTextChanged(const QString&)));

    this->searchEditAction = this->toolbar->addWidget (this->searchEdit);
    this->searchEditAction->setVisible (false);

    return (true);
}

bool
ContactWidget::onSearchTextChanged (
                                   QString filter)
{

    this->table->setSortingEnabled (false);

    for ( int row = 0; row < this->table->rowCount(); ++row )
    {

        if (this->tree->currentItem ()->statusTip (0).contains (
                                                               this->table->item (row, Contact::Type)->text ()))
        {

            bool match = false;

            for ( int column = 0; column < this->table->columnCount(); column++ )
            {

                if (false == this->table->isColumnHidden (column))
                {

                    if (this->table->item (row, column)->text ().contains (
                                                                          filter, Qt::CaseInsensitive))
                    {

                        match = true;
                        break;
                    }
                }
            }

            this->table->setRowHidden( row, !match );
        }
    }

    this->table->setSortingEnabled (true);

    return (true);
}

/// Context Menu
bool
ContactWidget::createContextMenu ()
{

    this->menu = new QMenu;

    this->menu->addAction (this->action.at (Action::New));
    this->menu->addAction (this->action.at (Action::Edit));
    this->menu->addAction (this->action.at (Action::Delete));
    this->menu->addAction (this->separator.at (Separator::One));

    this->menu->addAction (this->action.at (Action::Call));
    this->menu->addAction (this->action.at (Action::Message));
    this->menu->addAction (this->action.at (Action::Send));
    this->menu->addAction (this->separator.at (Separator::Two));

    this->menu->addAction (this->action.at (Action::Import));
    this->menu->addAction (this->action.at (Action::Export));
    this->menu->addAction (this->separator.at (Separator::Three));

    this->menu->addAction (this->action.at (Action::MoveToApplication));
    this->menu->addAction (this->action.at (Action::MoveToSim));

    this->menu->addAction (this->action.at (Action::CopyToApplication));
    this->menu->addAction (this->action.at (Action::CopyToSim));

    return (true);
}

bool
ContactWidget::onContextMenu (
                             QPoint point)
{

    QTableWidgetItem * dataItem = this->table->itemAt (point);

    bool contact = (NULL != dataItem);
    bool sim =
    ((true == contact) &&
     (Contact::String.at (Contact::Sim).contains (
                                                 this->table->item (dataItem->row (),
                                                                    Contact::Type)->text ())));

    this->action[Action::New]->setEnabled (true);
    this->action[Action::Edit]->setEnabled (true == contact);
    this->action[Action::Delete]->setEnabled (true == contact);

    this->action[Action::Call]->setEnabled (true == contact);
    this->action[Action::Message]->setEnabled (true == contact);
    this->action[Action::Send]->setEnabled (true == contact);

    this->action[Action::Export]->setEnabled (true == contact);
    this->action[Action::Import]->setEnabled (true);

    this->action[Action::Find]->setEnabled (true);

    this->action[Action::MoveToApplication]->setVisible (
                                                        (true == contact) && (true == sim));
    this->action[Action::MoveToSim]->setVisible (
                                                (true == contact) && (false == sim));

    this->action[Action::CopyToApplication]->setVisible (
                                                        (true == contact) && (true == sim));
    this->action[Action::CopyToSim]->setVisible (
                                                (true == contact) && (false == sim));

    this->menu->exec (this->mapToGlobal (point));
    return (true);
}

/// Table
bool ContactWidget::createTable ()
{
    this->table = new QTableWidget;
    this->table->setShowGrid (false);

    this->table->setColumnCount (Contact::FieldCount);
    this->table->setColumnWidth (Contact::Name, 100);
    this->table->setColumnHidden (Contact::Type, true);
    this->table->setColumnHidden (Contact::Index, true);

    this->table->setSelectionBehavior (QAbstractItemView::SelectRows);
    this->table->setHorizontalHeaderLabels (Contact::Header);

    this->table->verticalHeader ()->setDefaultSectionSize (18);
    this->table->verticalHeader ()->hide ();
    this->table->horizontalHeader ()->setStretchLastSection (true);

    this->table->setContextMenuPolicy (Qt::CustomContextMenu);

    this->load ();

    this->table->addActions (this->action);

    this->content = new ContactContentWidget;

    QObject::connect (this->table,
                      SIGNAL(currentItemChanged(
                                               QTableWidgetItem*,
                                               QTableWidgetItem*)),
                      this,
                      SLOT(onTableCurrentItemChanged(
                                                    QTableWidgetItem*,
                                                    QTableWidgetItem*)));


    QObject::connect (this->table,
                      SIGNAL(customContextMenuRequested(const QPoint&)),
                      this,
                      SLOT(onContextMenu(const QPoint&)));

    return (true);
}

bool
ContactWidget::onTableCurrentItemChanged (
                                         QTableWidgetItem* current,
                                         QTableWidgetItem* previous)
{

    Q_UNUSED (previous);

    if (NULL == current)
    {

        this->content->clear ();
        return (false);

    }
    else
    {

        this->content->load (this->rowToContact (this->table->currentRow ()));

        return (true);
    }
}

bool
ContactWidget::createLayout ()
{

    this->reader = new QSplitter(Qt::Vertical);
    this->reader->setChildrenCollapsible (false);

    this->reader->addWidget (this->toolbar);
    this->reader->addWidget (this->table);
    this->reader->addWidget (this->content);

    this->reader->setStretchFactor (1, 0);
    this->reader->setStretchFactor (2, 1);
    this->reader->setStretchFactor (3, 1);

    this->reader->setSizes (QList <int> () << 36 << 200 << 164 );

    this->splitter = new QSplitter(Qt::Horizontal);
    this->splitter->setChildrenCollapsible (false);

    this->splitter->addWidget(this->tree);
    this->splitter->addWidget(this->reader);

    this->splitter->setStretchFactor (1, 1);
    this->splitter->setStretchFactor (2, 1);

    this->splitter->setSizes (QList <int> () << 150 << 600);

    this->layout = new QVBoxLayout;
    this->layout->setContentsMargins (0, 0, 0, 0);
    this->layout->addWidget (this->splitter);

    this->setLayout (this->layout);

    return (true);
}

bool
ContactWidget::connect ()
{

    QObject::connect (DeamonInterface::getInstance (),
                      SIGNAL(contactCountUpdate (QString, QString)),
                      this,
                      SLOT(onContactCountUpdate (QString, QString)));

    QObject::connect (DeamonInterface::getInstance (),
                      SIGNAL(writeContactUpdate (QString)),
                      this,
                      SLOT(onWriteContactUpdate (QString)));

    QObject::connect (DeamonInterface::getInstance (),
                      SIGNAL(readContactUpdate (Contact)),
                      this,
                      SLOT(addUpdate (Contact)));

    QObject::connect (DeamonInterface::getInstance (),
                      SIGNAL(modemPlugged (bool)),
                      this,
                      SLOT(onModemPlugged(bool)));

    QObject::connect (DeamonConnection::getInstance (),
                      SIGNAL(modemConnected(bool)),
                      this,
                      SLOT(onModemConnected (bool)));

    return (true);
}

/// Helper
bool
ContactWidget::updateTable  ()
{

    this->table->setSortingEnabled (false);

    for (int row = 0; row < this->table->rowCount (); row++)
    {

        this->table->setRowHidden (row, !this->tree->currentItem ()
                                   ->statusTip (0).contains (
                                                            this->table->item (
                                                                              row, Contact::Type)->text ()));
    }

    this->table->setSortingEnabled (true);
    this->table->setCurrentCell (-1, -1);

    return (true);
}

bool ContactWidget::updateTree ()
{

    this->table->setSortingEnabled (false);

    for (int index = 0; index < this->item.count (); index++)
    {
        if (Contact::Sim == index)
        {
            continue;
        }

        int count = 0;

        for (int row = 0; row < this->table->rowCount (); row++)
        {

            if (this->item.at (index)->statusTip (0).contains (

                                                              this->table->item (row, Contact::Type)->text ()))
            {

                count++;
            }
        }

        this->item.at (index)->setText (0, Contact::Display.at (index)
                                        + " (" + QString::number (count) +")");


    }

    this->table->setSortingEnabled (true);

    return (true);
}


bool
ContactWidget::save (
                    QString number)
{

    Contact contact;

    contact.setMobile (number);
    contact.setType (Contact::Family);

    ContactDialog dialog(contact);

    if (QDialog::Accepted == dialog.exec ())
    {

        this->addUpdate (dialog.getContact ());
        return (true);

    }
    else
    {

        return (false);
    }
}

Contact
ContactWidget::rowToContact (
                            int index)
{

    return (Contact(this->table->item (index, Contact::Name)->text (),
                    this->table->item (index, Contact::Mobile)->text (),
                    this->table->item (index, Contact::Office)->text (),
                    this->table->item (index, Contact::Home)->text (),
                    this->table->item (index, Contact::Email)->text (),
                    this->table->item (index, Contact::Remark)->text (),
                    static_cast<Contact::Types>(
                                               this->table->item (index,
                                                                  Contact::Type)->text ().toInt ()),
                    this->table->item (index, Contact::Index)->text ().toInt ()));
}

bool
ContactWidget::contactToRow (
                            Contact contact)
{

    int row = this->table->rowCount ();
    this->table->insertRow (row);

    QTableWidgetItem * dataItem = NULL;

    dataItem = new QTableWidgetItem (contact.getName ());
    dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    dataItem->setTextAlignment (Qt::AlignLeft);
    dataItem->setIcon (this->treeIcon.at (contact.getType ()));
    this->table->setItem (row, Contact::Name, dataItem);

    dataItem = new QTableWidgetItem (contact.getMobile ());
    dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    dataItem->setTextAlignment (Qt::AlignLeft);
    this->table->setItem (row, Contact::Mobile, dataItem);

    dataItem = new QTableWidgetItem (contact.getOffice ());
    dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    dataItem->setTextAlignment (Qt::AlignLeft);
    this->table->setItem (row, Contact::Office, dataItem);

    dataItem = new QTableWidgetItem (contact.getHome ());
    dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    dataItem->setTextAlignment (Qt::AlignLeft);
    this->table->setItem (row, Contact::Home, dataItem);

    dataItem = new QTableWidgetItem (contact.getEmail ());
    dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    dataItem->setTextAlignment (Qt::AlignLeft);
    this->table->setItem (row, Contact::Email, dataItem);

    dataItem = new QTableWidgetItem (contact.getRemark ());
    dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    dataItem->setTextAlignment (Qt::AlignLeft);
    this->table->setItem (row, Contact::Remark, dataItem);

    dataItem = new QTableWidgetItem (QString::number (
                                                     static_cast<int> (contact.getType ())));
    dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    dataItem->setTextAlignment (Qt::AlignLeft);
    this->table->setItem (row, Contact::Type, dataItem);

    dataItem = new QTableWidgetItem (QString::number (contact.getIndex ()));
    dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    dataItem->setTextAlignment (Qt::AlignLeft);
    this->table->setItem (row, Contact::Index, dataItem);

    if (false == contact.getMobile ().isEmpty ())
    {

        emit name (contact.getMobile (), contact.getName ());
    }

    if (false == contact.getOffice ().isEmpty ())
    {

        emit name (contact.getOffice (), contact.getName ());
    }

    if (false == contact.getHome ().isEmpty ())
    {

        emit name (contact.getHome (), contact.getName ());
    }


    return (true);
}

bool
ContactWidget::load ()
{

    this->table->setSortingEnabled (false);

    QList <Contact> contacts = DeamonDatabase::getInstance ()->readContact ();

    foreach (Contact contact, contacts)
    {

        this->contactToRow (contact);
    }

    this->updateTree ();
    this->table->setSortingEnabled (true);

    return (true);
}

bool
ContactWidget::addUpdate (
                         Contact contact)
{

    this->table->setSortingEnabled (false);

    this->add (contact);

    this->updateTable ();
    this->updateTree ();

    this->table->setSortingEnabled (false);

    return (true);
}

bool
ContactWidget::add (Contact contact)
{

    if (contact.getType () != Contact::Sim)
    {

        contact.setIndex (DeamonDatabase::getInstance ()->addContact (contact));
    }

    this->contactToRow (contact);

    return (true);
}

bool
ContactWidget::edit (const Contact& contact)
{

    if (contact.getType () != Contact::Sim)
    {

        DeamonDatabase::getInstance()->editContact (contact);

        this->contactToRow (contact);

    }
    else
    {

        DeamonInterface::getInstance ()->write (
                                               At::editContact + QString::number (contact.getIndex ()) + COMMA +
                                               QUOTES + contact.getMobile () + QUOTES + COMMA +
                                               COMMA + QUOTES + contact.getName () +QUOTES);

        DeamonInterface::getInstance ()->write (
                                               At::contactCount);
    }

    this->updateTable ();
    this->updateTree ();

    return (true);
}

bool
ContactWidget::remove (int row)
{

    if (Contact::String.at (Contact::Local).contains (
                                                     this->table->item (row, Contact::Type)->text ()))
    {

        DeamonDatabase::getInstance ()->removeContact (
                                                      this->table->item (row, Contact::Index)->text ().toInt ());

    }
    else
    {

        DeamonInterface::getInstance ()->write (
                                               At::deleteContact + this->table->item (
                                                                                     row, Contact::Index)->text ());

        DeamonInterface::getInstance ()->write (At::contactCount);
    }

    this->table->removeRow (row);

    return (true);
}

bool
ContactWidget::read (
                    const QList<bool> &fields,
                    const QString &string)
{

    QStringList list = string.split (COMMA);
    Contact contact;

    if (fields.at (Contact::Name))
    {
        contact.setName (list.at (0));
        list.removeFirst ();
    }

    if (fields.at (Contact::Mobile))
    {
        contact.setMobile (list.at (0));
        list.removeFirst ();
    }

    if (fields.at (Contact::Office))
    {
        contact.setOffice (list.at (0));
        list.removeFirst ();
    }

    if (fields.at (Contact::Home))
    {
        contact.setHome (list.at (0));
        list.removeFirst ();
    }

    if (fields.at (Contact::Email))
    {
        contact.setEmail (list.at (0));
        list.removeFirst ();
    }

    if (fields.at (Contact::Remark))
    {
        contact.setRemark (list.at (0));
        list.removeFirst ();
    }

    if (fields.at (Contact::Type))
    {
        contact.setType (static_cast<Contact::Types> (list.at (0).toInt ()));
        list.removeFirst ();
    }
    else
    {
        contact.setType (Contact::Other);
    }

    if (Contact::Sim == contact.getType ())
    {

        DeamonInterface::getInstance ()->write (
                                               At::writeContact + QUOTES +
                                               contact.getMobile () +
                                               + QUOTES + COMMA + COMMA + QUOTES +
                                               contact.getName () +QUOTES);

    }
    else
    {

        this->add (contact);
    }

    return (true);
}

QString
ContactWidget::write (
                     const QList<bool>& fields,
                     int row)
{

    QString result;

    for (int index = 0; index < fields.count (); index++)
    {

        if (true == fields.at (index))
        {

            result += this->table->item (row, index)->text () + COMMA;
        }
    }

    return (result.replace (result.lastIndexOf (COMMA), 1, NEW_LINE_CARRIAGE));
}

bool
ContactWidget::onContactCountUpdate (
                                    QString count,
                                    QString total)
{

    this->item[Contact::Sim]->setText (0, Contact::Display.at (Contact::Sim) +
                                       + " (" + count + SLASH + total + ")");

    return (true);
}

bool
ContactWidget::onWriteContactUpdate (
                                    QString index)
{

    DeamonInterface::getInstance ()->write (
                                           At::readContact + index);

    return (true);
}

bool
ContactWidget::onModemPlugged (
                              bool status)
{

    this->action[Action::Call]->setEnabled (true == status);
    this->action[Action::Send]->setEnabled (true == status);
    this->action[Action::Message]->setEnabled (true == status);

    this->item[Contact::Sim]->setHidden (false == status);


    if (true == status)
    {

        DeamonInterface::getInstance ()->write (
                                               At::contactCount);

        DeamonInterface::getInstance ()->write (
                                               At::readAllContact);

    }
    else
    {

        this->table->setSortingEnabled (false);

        for (int index = this->table->rowCount () - 1; index >= 0; index--)
        {

            if (Contact::String.at (Contact::Sim).contains (
                                                           this->table->item (index, Contact::Type)->text ()))
            {
                this->table->removeRow (index);
            }
        }

        this->table->setSortingEnabled (true);
    }

    return (true);
}

bool
ContactWidget::onModemConnected (
                                bool status)
{
    Q_UNUSED (status);

    return (true);
}

bool
ContactWidget::number (
                      QString number)
{

    if (3 > number.length ())
    {

        return (false);
    }

    QString value(number.right (10));

    for (int row = 0; row < this->table->rowCount (); row++)
    {

        for (int column = 1; column < 4; column++)
        {

            if (this->table->item (row, column)->text ().contains (value))
            {

                emit name (number, this->table->item (row, 0)->text ());
            }
        }
    }

    return (false);
}

bool
ContactWidget::moveToApplication (
                                 int index)
{

    this->copyToApplication (index);
    this->remove (index);

    return (true);
}

bool
ContactWidget::moveToSim (
                         int index)
{

    this->copyToSim (index);
    this->remove (index);

    return (true);
}

bool
ContactWidget::copyToApplication (
                                 int index)
{

    Contact contact = this->rowToContact (index);
    contact.setType (Contact::Other);

    this->add (contact);

    return (true);
}

bool
ContactWidget::copyToSim (
                         int index)
{


    if (false == this->table->item (
                                   index, Contact::Mobile)->text ().isEmpty ())
    {

        DeamonInterface::getInstance ()->write (
                                               At::writeContact + QUOTES +
                                               this->table->item (index, Contact::Mobile)->text ()
                                               + QUOTES + COMMA + COMMA + QUOTES +
                                               this->table->item (index, Contact::Name)->text () +QUOTES);
    }

    if (false == this->table->item (
                                   index, Contact::Office)->text ().isEmpty ())
    {

        DeamonInterface::getInstance ()->write (
                                               At::writeContact + QUOTES +
                                               this->table->item (index, Contact::Office)->text ()
                                               + QUOTES + COMMA + COMMA + QUOTES +
                                               this->table->item (index, Contact::Name)->text ()+QUOTES);
    }

    if (false == this->table->item (
                                   index, Contact::Home)->text ().isEmpty ())
    {

        DeamonInterface::getInstance ()->write (
                                               At::writeContact + QUOTES +
                                               this->table->item (index, Contact::Home)->text ()
                                               + QUOTES + COMMA + COMMA + QUOTES +
                                               this->table->item (index, Contact::Name)->text ()+QUOTES);
    }

    return (true);
}

bool
ContactWidget::initialize ()
{

    this->treeIcon = QList<QIcon> ()
                     << QIcon(":/localSelected.png")
                     << QIcon(":/local.png")
                     << QIcon(":/simUsim.png")
                     << QIcon(":/family.png")
                     << QIcon(":/friends.png")
                     << QIcon(":/colleague.png")
                     << QIcon(":/other.png");

    this->actionName = QStringList ()
                       << tr("&New")
                       << tr("&Edit")
                       << tr("&Delete")
                       << tr("&Call")
                       << tr("&Message")
                       << tr("&Send")
                       << tr("&Search")
                       << tr("&Import")
                       << tr("&Export")
                       << tr("Move to &Local")
                       << tr("Move to &SIM")
                       << tr("Copy to &Local")
                       << tr("Copy to &SIM");

    this->actionToolTip = QStringList ()
                          << tr("Create New Contact")
                          << tr("Edit Contact")
                          << tr("Delete Contact")
                          << tr("Call Contact")
                          << tr("Send Message to Contact")
                          << tr("Send Contact")
                          << tr("Search Contact")
                          << tr("Import Contact")
                          << tr("Export Contact")
                          << tr("Move Contact to Local")
                          << tr("Move Contact to SIM")
                          << tr("Copy Contact to Local")
                          << tr("Copy Contact to SIM");

    this->actionIcon = QList<QIcon> ()
                       << QIcon(":/newContact.png")
                       << QIcon(":/editContact.png")
                       << QIcon(":/deleteContact.png")
                       << QIcon(":/callNumber.png")
                       << QIcon(":/sendMessage.png")
                       << QIcon(":/sendContact.png")
                       << QIcon(":/searchContact.png")
                       << QIcon(":/importContact.png")
                       << QIcon(":/exportContact.png")
                       << QIcon(":/moveMessageApplication.png")
                       << QIcon(":/moveMessageSim.png")
                       << QIcon(":/copyMessageApplication.png")
                       << QIcon(":/copyMessageSim.png");

    this->actionShortcut = QList<QKeySequence> ()
                           << QKeySequence(QKeySequence::New)
                           << QKeySequence(Qt::CTRL + Qt::Key_E)
                           << QKeySequence(QKeySequence::Delete)
                           << QKeySequence(Qt::CTRL + Qt::Key_D)
                           << QKeySequence(Qt::CTRL + Qt::Key_M)
                           << QKeySequence(QKeySequence::Copy)
                           << QKeySequence(QKeySequence::Find)
                           << QKeySequence(QKeySequence::Open)
                           << QKeySequence(QKeySequence::Save)
                           << Qt::CTRL + Qt::Key_L
                           << Qt::CTRL + Qt::Key_U
                           << Qt::CTRL + Qt::ALT + Qt::Key_L
                           << Qt::CTRL + Qt::ALT + Qt::Key_U;
    return (true);
}

bool ContactWidget::onReadContactUpdate (bool enable)
{

    if (true == enable)
    {

        QObject::connect (DeamonInterface::getInstance (),
                          SIGNAL(readContactUpdate(const Contact&)),
                          this,
                          SLOT(addUpdate (const Contact&)));

    }
    else
    {

        QObject::disconnect (DeamonInterface::getInstance (),
                             SIGNAL(readContactUpdate(const Contact&)),
                             this,
                             SLOT(addUpdate (const Contact&)));

    }

    return (true);
}
