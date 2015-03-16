#ifndef CONTACT_MULTIPLE_SELECT_DIALOG
#define CONTACT_MULTIPLE_SELECT_DIALOG

#include "Contact.h"

#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QSplitter>
#include <QtCore/QStringList>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QVBoxLayout>

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
