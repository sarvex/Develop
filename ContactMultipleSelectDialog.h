#ifndef CONTACT_MULTIPLE_SELECT_DIALOG
#define CONTACT_MULTIPLE_SELECT_DIALOG

#include "Contact.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSplitter>
#include <QStringList>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

class ContactMultipleSelectDialog
: public QDialog
{
    Q_OBJECT

public:
    explicit ContactMultipleSelectDialog (QWidget *parent = 0);

    QStringList getNumbers ();

    signals:

public slots:
    bool onOkClicked ();
    bool onCancelClicked ();

    bool onTreeItemDoubleClicked (QTreeWidgetItem* item, int column);
    bool onListItemDoubleClicked (QListWidgetItem* item);

    bool onReadContactUpdate (const Contact& contact);

protected:

private:
    bool createTree ();
    bool createList ();
    bool createLayout ();

    QTreeWidget * tree;
    QListWidget * list;

    QLabel * addContactHelp;
    QLabel * removeContactHelp;

    QDialogButtonBox * buttons;

    QSplitter * splitter;
    QHBoxLayout * helpLayout;
    QVBoxLayout * layout;

    QList <Contact> contacts;
};

#endif // CONTACT_MULTIPLE_SELECT_DIALOG
