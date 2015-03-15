#ifndef CONTACT_WIDGET
#define CONTACT_WIDGET

#include "Contact.h"
#include "ContactContentWidget.h"

#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <QList>
#include <QMenu>
#include <QPoint>
#include <QSplitter>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QToolBar>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

class ContactWidget
: public QWidget
{
    Q_OBJECT

public:
    ContactWidget (QWidget *parent = 0);
    bool save (QString number);

    signals:
    bool dial (QString number,
               bool connect);
    bool send (QString number,
               QString content);
    bool name (QString number,
               QString name);

public slots:
    bool onReadContactUpdate (bool enable);
    bool number (QString number);

    bool addUpdate (Contact contact);
    bool add (Contact contact);

    bool onTreeCurrentItemChanged (QTreeWidgetItem* current,
                                   QTreeWidgetItem* previous);

    bool onContextMenu (QPoint point);
    bool onTableCurrentItemChanged (QTableWidgetItem* current,
                                    QTableWidgetItem* previous);

    bool onSearchTextChanged (QString filter);

    bool onNewTriggered ();
    bool onEditTriggered ();
    bool onDeleteTriggered ();
    bool onCallTriggered ();
    bool onMessageTriggered ();
    bool onSendTriggered ();
    bool onFindTriggered ();
    bool onImportTriggered ();
    bool onExportTriggered ();

    bool onMoveToApplicationTriggered ();
    bool onMoveToSimTriggered ();
    bool onCopyToApplicationTriggered ();
    bool onCopyToSimTriggered ();

    bool onContactCountUpdate (QString count, QString total);
    bool onWriteContactUpdate (QString index);

    bool onModemPlugged(bool status);
    bool onModemConnected(bool status);

protected:

private:
    bool createActions ();
    bool createContextMenu ();
    bool createToolBar ();

    bool createTree ();
    bool createTable ();
    bool createLayout ();

    bool initialize ();
    bool load ();
    bool connect ();

    bool edit (const Contact& contact);
    bool remove (int row);

    bool contactToRow (Contact contact);
    Contact rowToContact (int row);

    bool updateTable ();
    bool updateTree ();

    bool moveToApplication (int row);
    bool moveToSim (int row);

    bool copyToApplication (int row);
    bool copyToSim (int row);

    QString write (const QList<bool>& fields, int row);
    bool read (const QList<bool>& fields, const QString& string);

    QTreeWidget * tree;
    QList <QTreeWidgetItem *> item;

    QToolBar * toolbar;
    QTableWidget * table;

    QMenu * menu;
    QList<QAction*> action;
    QList<QAction*> separator;

    QLineEdit * searchEdit;
    QAction * searchEditAction;

    ContactContentWidget * content;

    QSplitter * splitter;
    QSplitter * reader;
    QVBoxLayout * layout;

    QStringList header;
    QStringList toolTip;
    QList <QIcon> treeIcon;

    QStringList actionName;
    QStringList actionToolTip;
    QList <QIcon> actionIcon;
    QList <QKeySequence> actionShortcut;

    class Action
    {
    public:
        enum
        {
            New,
            Edit,
            Delete,
            Call,
            Message,
            Send,
            Find,
            Import,
            Export,
            MoveToApplication,
            MoveToSim,
            CopyToApplication,
            CopyToSim,
            Count
        };
    };

    class Separator
    {
    public:
        enum
        {
            One,
            Two,
            Three,
            Count
        };
    };
};

#endif // CONTACT_WIDGET
