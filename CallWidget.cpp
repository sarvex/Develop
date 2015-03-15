#include "CallWidget.h"
#include "DeamonConnection.h"
#include "DeamonDatabase.h"
#include "DeamonInterface.h"
#include "Define.h"

#include <QHeaderView>
#include <QMessageBox>

CallWidget::CallWidget(
                      QWidget *parent)
: QWidget(parent)
{

    this->initialize ();

    this->createActions ();
    this->createContextMenu ();

    this->createTree ();
    this->createTable ();

    this->createLayout ();

    this->connect ();
}

/// Actions
bool
CallWidget::createActions ()
{

    for (int index = 0; index < Action::Count; index++)
    {

        this->action << new QAction (
                                    this->actionIcon.at (index),
                                    this->actionName.at (index),
                                    this);

        this->action[index]->setShortcut (this->actionShortcut.at (index));
        this->action[index]->setToolTip (this->actionTooltip.at (index));
    }

    for (int index = 0; index < Separator::Count; index++)
    {

        this->separator << new QAction (this);
        this->separator[index]->setSeparator (true);
    }

    QObject::connect (this->action.at (Action::Call),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onCallNumberTriggered()));

    QObject::connect (this->action.at (Action::Message),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onMessageNumberTriggered()));

    QObject::connect (this->action.at (Action::Save),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onSaveNumberTriggered()));

    QObject::connect (this->action.at (Action::Delete),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onDeleteNumberTriggered()));

    QObject::connect (this->action.at (Action::Clear),
                      SIGNAL(triggered()),
                      this,
                      SLOT(onClearNumberTriggered()));

    return(true);
}

bool
CallWidget::onCallNumberTriggered ()
{

    int row = this->table->currentRow ();
    if ( 0 > row)
    {

        QMessageBox::warning (NULL,
                              NO_SELECTION,
                              NO_SELECTION_CALL,
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return(false);
    }

    emit this->dial (this->table->item (row, Call::Number)->text (), "");

    return(true);
}

bool
CallWidget::onMessageNumberTriggered ()
{

    int row = this->table->currentRow ();
    if ( 0 > row)
    {

        QMessageBox::warning (NULL,
                              NO_SELECTION,
                              NO_SELECTION_CALL,
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return(false);
    }


    emit this->send (this->table->item (row, Call::Number)->text (), "");

    return(true);
}

bool
CallWidget::onSaveNumberTriggered ()
{

    int row = this->table->currentRow ();
    if ( 0 > row)
    {

        QMessageBox::warning (NULL,
                              NO_SELECTION,
                              NO_SELECTION_CONTACT,
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return(false);
    }

    emit this->save (this->table->item (row, Call::Number)->text ());

    return(true);
}

bool CallWidget::onDeleteNumberTriggered ()
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

    return(true);
}

bool
CallWidget::onClearNumberTriggered ()
{

    this->table->setSortingEnabled (false);

    QList<int> indicies;

    for (int row = 0; row < this->table->rowCount (); row++)
    {

        if (false == this->table->isRowHidden (row))
        {
            indicies << row;
        }
    }

    qSort (indicies.begin (), indicies.end (), qGreater<int> ());

    foreach (int index, indicies)
    {
        this->remove (index);
    }

    this->updateTable ();
    this->updateTree ();

    this->table->setSortingEnabled (true);

    return(true);
}

bool
CallWidget::createTree ()
{

    this->tree = new QTreeWidget;
    this->tree->setColumnCount (1);
    this->tree->setHeaderLabel (tr("Call History"));

    this->item << new QTreeWidgetItem (this->tree);

    this->item << new QTreeWidgetItem (this->item.at (Call::All));
    this->item << new QTreeWidgetItem (this->item.at (Call::All));
    this->item << new QTreeWidgetItem (this->item.at (Call::All));

    this->item.at (Call::All)->setExpanded (true);
    this->tree->setCurrentItem (this->item.at (Call::All));

    for (int index = 0; index < this->item.count (); index++)
    {

        this->item[index]->setText (0, Call::Display.at (index));
        this->item[index]->setIcon (0, this->icon.at (index));
        this->item[index]->setToolTip (0, Call::ToolTip.at (index));
        this->item[index]->setStatusTip (0, Call::String.at (index));
    }

    this->tree->setCurrentItem (this->item.at (Call::All));

    QObject::connect (this->tree,
                      SIGNAL(currentItemChanged(
                                               QTreeWidgetItem*,
                                               QTreeWidgetItem*)),
                      this,
                      SLOT(onTreeCurrentItemChanged(
                                                   QTreeWidgetItem*,
                                                   QTreeWidgetItem*)));

    return(true);
}

bool
CallWidget::onTreeCurrentItemChanged (
                                     QTreeWidgetItem* current,
                                     QTreeWidgetItem* previous)
{
    Q_UNUSED (current);
    Q_UNUSED (previous);

    this->updateTable ();

    return(true);
}


bool
CallWidget::createTable ()
{

    this->table = new QTableWidget;
    this->table->setShowGrid (false);
    this->table->setSelectionBehavior (QAbstractItemView::SelectRows);

    this->table->setColumnCount (Call::FieldCount);
    this->table->setColumnWidth (Call::Name, 150);

    this->table->setColumnHidden (Call::Number, true);
    this->table->setColumnHidden (Call::Index, true);
    this->table->setColumnHidden (Call::Type, true);
    this->table->setColumnHidden (Call::Unread, true);

    this->table->verticalHeader ()->setDefaultSectionSize (18);
    this->table->verticalHeader ()->setVisible (false);

    this->table->horizontalHeader ()->setStretchLastSection (true);
    this->table->setHorizontalHeaderLabels (Call::Header);

    this->table->setContextMenuPolicy (Qt::CustomContextMenu);

    this->load ();

    this->table->sortByColumn (Call::StartTime, Qt::DescendingOrder);

    this->table->addActions (this->action);

    QObject::connect (this->table,
                      SIGNAL(customContextMenuRequested (const QPoint&)),
                      this,
                      SLOT(onContextMenu (const QPoint&)));

    QObject::connect (this->table,
                      SIGNAL(currentItemChanged (QTableWidgetItem*,
                                                 QTableWidgetItem*)),
                      this,
                      SLOT(onTableCurrentItemChanged (QTableWidgetItem*,
                                                      QTableWidgetItem*)));

    this->updateTree ();
    return(true);
}


bool
CallWidget::createContextMenu ()
{

    this->menu = new QMenu;

    this->menu->addAction (this->action.at (Action::Call));
    this->menu->addAction (this->action.at (Action::Message));
    this->menu->addAction (this->action.at (Action::Save));
    this->menu->addAction (this->separator.at (Separator::One));

    this->menu->addAction (this->action.at (Action::Delete));
    this->menu->addAction (this->action.at (Action::Clear));

    return(true);
}

bool
CallWidget::onContextMenu (
                          QPoint point)
{

    bool call = (NULL != this->table->itemAt (point));

    this->action.at (Action::Call)->setEnabled (true == call);
    this->action.at (Action::Message)->setEnabled (true == call);
    this->action.at (Action::Save)->setEnabled (true == call);
    this->action.at (Action::Delete)->setEnabled (true == call);
    this->action.at (Action::Clear)->setEnabled (true);

    this->menu->exec(this->mapToGlobal (point));

    return(true);
}

bool CallWidget::onTableCurrentItemChanged (
                                           QTableWidgetItem* current,
                                           QTableWidgetItem* previous)
{

    Q_UNUSED(current);
    Q_UNUSED(previous);

    QFont font = this->font ();
    font.setBold (false);

    int row = this->table->currentRow ();
    if (0 > row)
    {

        return(false);
    }

    if (true == this->table->item (row, Message::Unread)
        ->text ().contains (FALSE_INT))
    {
        return(false);
    }

    for (int column = 0; column < Call::FieldCount; column++)
    {

        this->table->item (row, column)->setFont (font);
    }

    this->table->item (row, Call::Unread)->setText (FALSE_INT);

    DeamonDatabase::getInstance ()->editCall (this->rowToCall (row));

    this->updateTree ();
    return(true);
}

bool
CallWidget::createLayout ()
{

    this->splitter = new QSplitter(Qt::Vertical);
    this->splitter->setChildrenCollapsible (false);

    this->splitter->addWidget (this->tree);
    this->splitter->addWidget (this->table);

    this->splitter->setStretchFactor (1, 1);
    this->splitter->setStretchFactor (2, 1);

    this->splitter->setSizes (QList <int> () << 100 << 500);

    this->layout = new QVBoxLayout;
    this->layout->setContentsMargins (0, 0, 0, 0);
    this->layout->addWidget (this->splitter);

    this->setLayout (this->layout);

    return(true);
}


bool
CallWidget::load ()
{

    this->table->setSortingEnabled (false);

    QList<Call> calls = DeamonDatabase::getInstance ()->readCall ();
    foreach (Call call, calls)
    {

        this->callToRow (call);
    }

    this->updateTree ();

    this->table->setSortingEnabled (true);
    return(true);
}

Call
CallWidget::rowToCall (
                      int index)
{

    return(Call (this->table->item (index, Call::Number)->text (),
                 this->table->item (index, Call::Name)->text (),
                 QDateTime::fromString (this->table->item (
                                                          index, Call::StartTime)->text (),
                                        DATETIME_FORMAT),
                 QTime::fromString (this->table->item (
                                                      index, Call::Duration)->text (),
                                    TIME_FORMAT),
                 static_cast<Call::Types> (this->table->item (
                                                             index, Call::Type)
                                           ->text ().toInt ()),
                 this->table->item (index, Call::Index)->text ().toInt (),
                 ( 1 == this->table->item (index, Call::Unread)
                   ->text ().toInt ())));
}

bool
CallWidget::callToRow (
                      Call call)
{

    int row = this->table->rowCount ();
    this->table->insertRow (row);

    QTableWidgetItem * dataItem = NULL;

    QFont font = this->font ();
    font.setBold (call.getUnread ());

    if (call.getName ().isEmpty ())
    {
        call.setName (call.getNumber ());
    }

    dataItem = new QTableWidgetItem(call.getNumber ());
    dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    dataItem->setFont (font);
    dataItem->setTextAlignment (Qt::AlignLeft);
    this->table->setItem (row, Call::Number, dataItem);

    dataItem = new QTableWidgetItem(call.getName ());
    dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    dataItem->setFont (font);
    dataItem->setTextAlignment (Qt::AlignLeft);
    dataItem->setIcon (this->icon.at (call.getType ()));
    this->table->setItem (row, Call::Name, dataItem);

    dataItem = new QTableWidgetItem(call.getStartTime ()
                                    .toString (DATETIME_FORMAT));
    dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    dataItem->setFont (font);
    dataItem->setTextAlignment (Qt::AlignLeft);
    this->table->setItem (row, Call::StartTime, dataItem);

    dataItem = new QTableWidgetItem(call.getDuration ()
                                    .toString (TIME_FORMAT));
    dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    dataItem->setFont (font);
    dataItem->setTextAlignment (Qt::AlignLeft);
    this->table->setItem (row, Call::Duration, dataItem);

    dataItem = new QTableWidgetItem(QString::number (
                                                    static_cast<int>(call.getType ())));
    dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    dataItem->setFont (font);
    dataItem->setTextAlignment (Qt::AlignLeft);
    this->table->setItem (row, Call::Type, dataItem);

    dataItem = new QTableWidgetItem(QString::number (call.getIndex ()));
    dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    dataItem->setFont (font);
    dataItem->setTextAlignment (Qt::AlignLeft);
    this->table->setItem (row, Call::Index, dataItem);

    dataItem = new QTableWidgetItem (QString::number (
                                                     static_cast<int>(call.getUnread ())));
    dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    dataItem->setFont (font);
    dataItem->setTextAlignment (Qt::AlignLeft);
    this->table->setItem (row, Call::Unread, dataItem);

    if (call.getName ().contains (call.getNumber ()))
    {

        emit this->number (call.getNumber ());
    }

    return(true);
}


bool
CallWidget::addUpdate (
                      Call call)
{

    this->table->setSortingEnabled (false);

    this->add (call);

    this->updateTree ();
    this->table->setSortingEnabled (true);

    return(true);
}

bool
CallWidget::add (
                Call call)
{

    call.setIndex (DeamonDatabase::getInstance ()->addCall (call));

    this->callToRow (call);

    return(true);
}


bool
CallWidget::remove (int index)
{

    DeamonDatabase::getInstance ()->removeCall (
                                               this->table->item (index, Call::Index)->text ().toInt ());

    this->table->removeRow (index);

    return(true);
}

bool
CallWidget::onModemPlugged (
                           bool status)
{

    this->action.at (Action::Call)->setEnabled (true == status);
    this->action.at (Action::Message)->setEnabled (true == status);

    return(true);
}

bool
CallWidget::onModemConnected (
                             bool status)
{
    Q_UNUSED (status);

    return(true);
}

bool
CallWidget::name (QString number, QString name)
{

    this->table->setSortingEnabled (false);

    for (int row = 0; row < this->table->rowCount (); row++)
    {

        if (this->table->item (row, Call::Number)->text ().contains (number))
        {

            this->table->item (row, Call::Name)->setText (name);
            DeamonDatabase::getInstance ()->editCall (this->rowToCall (row));
        }
    }

    this->table->setSortingEnabled (true);
    return(true);
}

bool
CallWidget::updateTable ()
{

    this->table->setSortingEnabled (false);

    for (int row = 0; row < this->table->rowCount (); row++)
    {

        this->table->setRowHidden (row, false == this->tree->currentItem ()
                                   ->statusTip (0).contains (
                                                            this->table->item (
                                                                              row, Call::Type)->text ()));
    }

    this->table->setSortingEnabled (true);
    this->table->setCurrentCell (-1, -1);

    return(true);
}

bool CallWidget::updateTree ()
{

    this->table->setSortingEnabled (false);

    int total = 0;

    for (int index = 0; index < this->item.count (); index++)
    {

        int count = 0;
        int unread = 0;

        for (int row = 0; row < this->table->rowCount (); row++)
        {

            if (this->item.at (index)->statusTip (0).contains (

                                                              this->table->item (row, Call::Type)->text ()))
            {

                count++;

                if (this->table->item (row, Call::Unread)
                    ->text ().contains (TRUE_INT))
                {
                    unread++;
                }
            }
        }

        this->item.at (index)->setText (0, Call::Display.at (index)
                                        + " (" + QString::number (unread) + "/"
                                        + QString::number (count) +")");

        total += unread;
    }

    emit callAnnunciator (0 != total);

    this->table->setSortingEnabled (true);

    return(true);
}

bool
CallWidget::onSelectUnread ()
{

    for (int index = Call::Answered; index < this->item.count (); index++)
    {

        QStringList list = this->item.at (index)
                           ->text (0).split (OPEN_PARANTHESIS);

        if ( (1 > list.count ()) || (list.at (1).startsWith ('0')) )
        {

            continue;

        }
        else
        {

            this->tree->setCurrentItem (this->item.at (index));
            break;
        }
    }

    return(true);
}

bool
CallWidget::connect ()
{

    QObject::connect (DeamonInterface::getInstance (),
                      SIGNAL(modemPlugged (bool)),
                      this,
                      SLOT(onModemPlugged (bool)));

    QObject::connect (DeamonConnection::getInstance (),
                      SIGNAL(modemConnected (bool)),
                      this,
                      SLOT(onModemConnected (bool)));

    return(true);
}

bool
CallWidget::initialize ()
{

    this->icon << QIcon (":/allCalls.png")
    << QIcon (":/answeredCalls.png")
    << QIcon (":/missedCalls.png")
    << QIcon (":/dialledCalls.png");

    this->actionName << tr("&Call")
    << tr("&Message")
    << tr("&Save")
    << tr("&Delete")
    << tr("C&lear");

    this->actionTooltip << tr("Call Number")
    << tr("Send Message to Number")
    << tr("Save Number")
    << tr("Delete Call")
    << tr("Clear All Calls");

    this->actionIcon << QIcon(":/call.png")
    << QIcon(":/message.png")
    << QIcon(":/save.png")
    << QIcon(":/delete.png")
    << QIcon(":/clear.png");

    this->actionShortcut << Qt::CTRL + Qt::Key_D
    << Qt::CTRL + Qt::Key_M
    << QKeySequence::Save
    << QKeySequence::Delete
    << Qt::CTRL + Qt::Key_L;

    return(true);
}
