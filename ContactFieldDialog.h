#ifndef CONTACT_FIELD_DIALOG
#define CONTACT_FIELD_DIALOG

#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>

class ContactFieldDialog
: public QDialog
{
    Q_OBJECT

public:
    explicit ContactFieldDialog (QWidget *parent = 0);
    QList <bool> getFields ();

    signals:

public slots:
    bool onOkClicked ();
    bool onCancelClicked ();

protected:

private:
    QListWidget * list;

    QListWidgetItem * nameItem;
    QListWidgetItem * mobileItem;
    QListWidgetItem * officeItem;
    QListWidgetItem * homeItem;
    QListWidgetItem * emailItem;
    QListWidgetItem * remarkItem;
    QListWidgetItem * typeItem;

    QCheckBox * name;
    QCheckBox * mobile;
    QCheckBox * office;
    QCheckBox * home;
    QCheckBox * email;
    QCheckBox * remark;
    QCheckBox * type;

    QDialogButtonBox * buttons;

    QVBoxLayout * layout;
};

#endif // CONTACT_FIELD_DIALOG
