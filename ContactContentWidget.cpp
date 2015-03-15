#include "ContactContentWidget.h"

ContactContentWidget::ContactContentWidget (
                                           QWidget *parent)
: QWidget (parent)
{

    this->nameLabel = new QLabel(tr("Name"));
    this->nameLabel->setAlignment (Qt::AlignRight);
    this->nameLabel->setToolTip (tr("Name"));

    this->name = new QLineEdit;
    this->name->setPlaceholderText (tr("Name..."));
    this->name->setReadOnly (true);

    this->mobileLabel = new QLabel(tr("Mobile"));
    this->mobileLabel->setAlignment (Qt::AlignRight);
    this->mobileLabel->setToolTip (tr("Mobile Number"));

    this->mobile = new QLineEdit;
    this->mobile->setPlaceholderText (tr("Mobile..."));
    this->mobile->setReadOnly (true);

    this->officeLabel = new QLabel(tr("Office"));
    this->officeLabel->setAlignment (Qt::AlignRight);
    this->officeLabel->setToolTip (tr("Office Number"));

    this->office = new QLineEdit;
    this->office->setPlaceholderText (tr("Office..."));
    this->office->setReadOnly (true);

    this->homeLabel = new QLabel(tr("Home"));
    this->homeLabel->setAlignment (Qt::AlignRight);
    this->homeLabel->setToolTip (tr("Home Number"));

    this->home = new QLineEdit;
    this->home->setPlaceholderText (tr("Home..."));
    this->home->setReadOnly (true);

    this->emailLabel = new QLabel(tr("Email"));
    this->emailLabel->setAlignment (Qt::AlignRight);
    this->emailLabel->setToolTip (tr("Email Address"));

    this->email = new QLineEdit;
    this->email->setPlaceholderText (tr("Email..."));
    this->email->setReadOnly (true);

    this->remarkLabel = new QLabel(tr("Remark"));
    this->remarkLabel->setAlignment (Qt::AlignRight);
    this->remarkLabel->setToolTip (tr("Remarks"));

    this->remark = new QLineEdit;
    this->remark->setPlaceholderText (tr("Remark..."));
    this->remark->setReadOnly (true);

    this->layout = new QGridLayout;
    this->layout->addWidget (this->nameLabel,  0, 0, 1, 1);
    this->layout->addWidget (this->name,  0, 1, 1, 1);
    this->layout->addWidget (this->mobileLabel,  0, 2, 1, 1);
    this->layout->addWidget (this->mobile,  0, 3, 1, 1);
    this->layout->addWidget (this->officeLabel,  1, 0, 1, 1);
    this->layout->addWidget (this->office,  1, 1, 1, 1);
    this->layout->addWidget (this->homeLabel,  1, 2, 1, 1);
    this->layout->addWidget (this->home,  1, 3, 1, 1);
    this->layout->addWidget (this->emailLabel,  2, 0, 1, 1);
    this->layout->addWidget (this->email,  2, 1, 1, 1);
    this->layout->addWidget (this->remarkLabel,  2, 2, 1, 1);
    this->layout->addWidget (this->remark,  2, 3, 1, 1);
    this->setLayout (this->layout);
}

bool
ContactContentWidget::load (
                           const Contact& contact)
{

    this->name->setText (contact.getName ());
    this->name->setToolTip (contact.getName ());

    this->mobile->setText (contact.getMobile ());
    this->mobile->setToolTip (contact.getMobile ());

    this->office->setText (contact.getOffice ());
    this->office->setToolTip (contact.getOffice ());

    this->home->setText (contact.getHome ());
    this->home->setToolTip (contact.getHome ());

    this->email->setText (contact.getEmail ());
    this->email->setToolTip (contact.getEmail ());

    this->remark->setText (contact.getRemark ());
    this->remark->setToolTip (contact.getRemark ());

    return (true);
}

bool
ContactContentWidget::clear ()
{

    this->name->clear ();
    this->name->setToolTip (tr("Name"));

    this->mobile->clear ();
    this->mobile->setToolTip (tr("Mobile Number"));

    this->office->clear ();
    this->office->setToolTip (tr("Office Number"));

    this->home->clear ();
    this->home->setToolTip (tr("Home Number"));

    this->email->clear ();
    this->email->setToolTip (tr("Email Address"));

    this->remark->clear ();
    this->remark->setToolTip (tr("Remarks"));


    return (true);

}
