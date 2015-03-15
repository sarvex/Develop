#ifndef CALL_WIDGET
#define CALL_WIDGET

#include "Call.h"

#include <QAction>
#include <QKeySequence>
#include <QList>
#include <QMenu>
#include <QPoint>
#include <QSplitter>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

class CallWidget
: public QWidget
{
    Q_OBJECT

public:
    explicit CallWidget (QWidget *parent = 0);

    bool updateTree ();

    signals:
    bool callAnnunciator (bool visible);

    bool number (const QString& number);

    bool save (QString number);
    bool dial (QString number,
               bool connect);
    bool send (QString number,
               QString content);

public slots:
    bool onSelectUnread ();

    bool add (Call call);
    bool addUpdate (Call call);

    bool name (QString number,
               QString name);

    bool onContextMenu (QPoint point);
    bool onTableCurrentItemChanged (
                                   QTableWidgetItem* current,
                                   QTableWidgetItem* previous);

    bool onTreeCurrentItemChanged (
                                  QTreeWidgetItem* current,
                                  QTreeWidgetItem* previous);


    bool onCallNumberTriggered ();
    bool onMessageNumberTriggered ();
    bool onSaveNumberTriggered ();
    bool onDeleteNumberTriggered ();
    bool onClearNumberTriggered ();

    bool onModemPlugged (bool status);
    bool onModemConnected (bool status);

protected:

private:
    bool createActions ();
    bool createContextMenu ();

    bool createTree ();
    bool createTable ();
    bool createLayout ();

    bool initialize ();
    bool load ();
    bool connect ();

    bool remove (int index);
    bool edit (const Call& call);

    bool callToRow (Call call);
    Call rowToCall (int index);

    bool updateTable ();

    QTreeWidget * tree;
    QList <QTreeWidgetItem*> item;

    QTableWidget * table;

    QMenu * menu;
    QList <QAction*> action;
    QList <QAction*> separator;

    QSplitter * splitter;
    QVBoxLayout * layout;

    QList <QIcon> icon;

    QStringList actionName;
    QStringList actionTooltip;
    QList <QIcon> actionIcon;
    QList <QKeySequence> actionShortcut;

    class Action
    {
    public:
        enum
        {
            Call,
            Message,
            Save,
            Delete,
            Clear,
            Count
        };
    };

    class Separator
    {
    public:
        enum
        {
            One,
            Count
        };
    };
};

#endif // CALL_WIDGET
