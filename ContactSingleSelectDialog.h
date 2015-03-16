#ifndef CONTACT_SINGLE_SELECT_DIALOG
#define CONTACT_SINGLE_SELECT_DIALOG

#include "Contact.h"

#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QSplitter>
#include <QtCore/QString>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QVBoxLayout>

class ContactSingleSelectDialog
: public QDialog
{
    Q_OBJECT

public:
    explicit ContactSingleSelectDialog(QWidget *parent = 0);
    QString getNumber ();

    signals:

public slots:
    bool onOkClicked ();
    bool onCancelClicked ();

    bool onTreeItemDoubleClicked (QTreeWidgetItem* item, int column);

    bool onReadContactUpdate (const Contact& contact);

protected:

private:
    QTreeWidget * tree;

    QDialogButtonBox * buttons;

    QSplitter * splitter;
    QVBoxLayout * layout;

    QString number;
};

#endif // CONTACT_SINGLE_SELECT_DIALOG
