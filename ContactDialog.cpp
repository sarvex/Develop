#include "ContactDialog.h"
#include "Define.h"

ContactDialog::ContactDialog (
                             const Contact& contact,
                             QWidget* parent)
: QDialog (parent)
{

    this->initialize (contact.getType ());

    this->name->setText (contact.getName ());
    this->mobile->setText (contact.getMobile ());
    this->office->setText (contact.getOffice ());
    this->home->setText (contact.getHome ());
    this->email->setText (contact.getEmail ());
    this->remark->setText (contact.getRemark ());

    this->type->setCurrentIndex (static_cast<int>(contact.getType())
                                 - static_cast<int> (Contact::Family));

    this->index = contact.getIndex ();
}

ContactDialog::ContactDialog (
                             QWidget *parent)
: QDialog(parent)
{

    this->initialize (Contact::Family);
}

bool
ContactDialog::initialize (Contact::Types type)
{

    this->setWindowFlags(Qt::WindowSystemMenuHint |
                         Qt::WindowTitleHint);
    this->setWindowIcon(QIcon(APPLICATION_ICON));
    this->setWindowTitle (NEW_CONTACT);

    this->validator = new QRegExpValidator(QRegExp("(\\+?\\d*)"), this);

    this->name = new QLineEdit;
    this->name->setPlaceholderText (tr("Name..."));
    this->name->setToolTip (tr("Name"));

    this->mobile = new QLineEdit;
    this->mobile->setPlaceholderText (tr("Mobile Number..."));
    this->mobile->setToolTip (tr("Mobile Number"));
    this->mobile->setValidator (this->validator);

    this->office = new QLineEdit;
    this->office->setPlaceholderText (tr("Office Number..."));
    this->office->setToolTip (tr("Office Number"));
    this->office->setValidator (this->validator);

    this->home = new QLineEdit;
    this->home->setPlaceholderText (tr("Home Number..."));
    this->home->setToolTip (tr("Home Number"));
    this->home->setValidator (this->validator);

    this->email = new QLineEdit;
    this->email->setPlaceholderText (tr("Email..."));
    this->email->setToolTip (tr("Email Address"));
    this->email->setValidator (new QRegExpValidator (
                                                    QRegExp("^[A-Za-z0-9._%+-]+"
                                                            "@[A-Za-z0-9.-]+"
                                                            "\\.[A-Za-z]{2,4}$"), this));

    this->remark = new QLineEdit;
    this->remark->setPlaceholderText (tr("Remark..."));
    this->remark->setToolTip (tr("Remark"));

    this->type = new QComboBox;
    this->type->addItems (QStringList ()
                          << tr("Family")
                          << tr("Friend")
                          << tr("Collegue")
                          << tr("Other"));

    this->buttons = new QDialogButtonBox (
                                         QDialogButtonBox::Ok |
                                         QDialogButtonBox::Cancel);

    this->formLayout = new QFormLayout;
    this->formLayout->setLabelAlignment (Qt::AlignRight);

    this->formLayout->addRow ("Name", this->name);

    if (Contact::Sim == type)
    {

        this->name->setMaxLength (SIM_NAME_LIMIT);
        this->name->setPlaceholderText (tr("Name"));
        this->name->setToolTip (tr("Maximum 12 characters"));

        this->mobile->setMaxLength (SIM_MOBILE_LIMIT);
        this->mobile->setPlaceholderText (tr("Number"));
        this->mobile->setToolTip (tr("Maximum 20 Characters"));

        this->formLayout->addRow (tr("Number"), this->mobile);

    }
    else
    {

        this->formLayout->addRow (tr("Mobile"), this->mobile);
        this->formLayout->addRow (tr("Office"), this->office);
        this->formLayout->addRow (tr("Home"), this->home);
        this->formLayout->addRow (tr("Email"), this->email);
        this->formLayout->addRow (tr("Remark"), this->remark);
        this->formLayout->addRow (tr("Group"), this->type);

    }

    this->layout = new QVBoxLayout;
    this->layout->addLayout (this->formLayout);
    this->layout->addWidget (this->buttons);

    this->setLayout (this->layout);

    QObject::connect (this->buttons,
                      SIGNAL(accepted()),
                      this,
                      SLOT(onOkClicked()));

    QObject::connect (this->buttons,
                      SIGNAL(rejected()),
                      this,
                      SLOT(onCancelClicked()));

    return (true);
}

bool
ContactDialog::onOkClicked ()
{

    this->done (QDialog::Accepted);

    return (true);
}

bool
ContactDialog::onCancelClicked ()
{

    this->done (QDialog::Rejected);

    return (true);
}

Contact
ContactDialog::getContact () const
{

    return (Contact(this->name->text (),
                    this->mobile->text (),
                    this->office->text (),
                    this->home->text (),
                    this->email->text (),
                    this->remark->text (),
                    static_cast<Contact::Types>(this->type->currentIndex ()
                                                + static_cast<int> (
                                                                   Contact::Family)),
                    this->index));
}
