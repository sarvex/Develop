#ifndef CONTACT_CONTENT_WIDGET
#define CONTACT_CONTENT_WIDGET

#include "Contact.h"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

class ContactContentWidget
: public QWidget
{
    Q_OBJECT

public:
    explicit ContactContentWidget (QWidget *parent = 0);

    bool load (const Contact& contact);

    bool clear ();

    signals:

public slots:

protected:

private:
    QLabel * nameLabel;
    QLabel * mobileLabel;
    QLabel * officeLabel;
    QLabel * homeLabel;
    QLabel * emailLabel;
    QLabel * remarkLabel;

    QLineEdit * name;
    QLineEdit * mobile;
    QLineEdit * office;
    QLineEdit * home;
    QLineEdit * email;
    QLineEdit * remark;

    QGridLayout * layout;
};

#endif // CONTACT_CONTENT_WIDGET
