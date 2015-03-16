#ifndef CONTACT_SELECT_DIALOG
#define CONTACT_SELECT_DIALOG

#include "Contact.h"

#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QListWidgetItem>
#include <QtCore/QString>
#include <QtWidgets/QVBoxLayout>

class ContactSelectDialog
        : public QDialog {
    Q_OBJECT

public:
    ContactSelectDialog (const Contact& contact,
                         QWidget* parent = 0);

    ContactSelectDialog (const QString& name,
                         const QString& mobile,
                         const QString& office,
                         const QString& home,
                         int defaultNumber,
                         QWidget* parent = 0);

    QString getNumber ();

signals:

public slots:
    bool onOkClicked ();
    bool onCancelClicked ();

    bool onListItemClicked (QListWidgetItem* item);
    bool onListItemDoubleClicked (QListWidgetItem* item);

protected:

private:
    bool initialize (const QString& name,
                     const QString& mobile,
                     const QString& office,
                     const QString& home);

    QListWidget * list;
    QListWidgetItem * mobile;
    QListWidgetItem * office;
    QListWidgetItem * home;

    QDialogButtonBox * buttons;

    QVBoxLayout * layout;

    QString number;
};

#endif // CONTACT_SELECT_DIALOG
