#ifndef CONTACT_DIALOG
#define CONTACT_DIALOG

#include "Contact.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QVBoxLayout>

class ContactDialog
: public QDialog
{
    Q_OBJECT

public:
    explicit ContactDialog (const Contact& contact, QWidget* parent = 0);

    explicit ContactDialog (QWidget *parent = 0);

    Contact getContact () const;

    signals:

public slots:
    bool onOkClicked ();
    bool onCancelClicked ();

protected:

private:
    bool initialize (Contact::Types type);

    QRegExpValidator * validator;

    QLineEdit * name;
    QLineEdit * mobile;
    QLineEdit * office;
    QLineEdit * home;
    QLineEdit * email;
    QLineEdit * remark;

    QComboBox * type;

    QDialogButtonBox * buttons;

    QFormLayout * formLayout;
    QVBoxLayout * layout;

    int index;
};

#endif // CONTACT_DIALOG
