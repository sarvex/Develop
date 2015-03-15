#include "Contact.h"

Contact::Contact ()
{
    this->type = Contact::Other;
    this->index = -1;
}

Contact::Contact (
                 const Contact& contact)
: name (contact.name),
mobile (contact.mobile),
office (contact.office),
home (contact.home),
email (contact.email),
remark (contact.remark),
type (contact.type),
index (contact.index)
{
}

Contact::Contact (
                 const QString& name,
                 const QString& mobile,
                 const QString& office,
                 const QString& home,
                 const QString& email,
                 const QString& remark,
                 Contact::Types type,
                 int index)
: name (name),
mobile (mobile),
office (office),
home (home),
email (email),
remark (remark),
type (type),
index (index)
{
}

Contact&
Contact::operator = (
                    const Contact& contact) {

    if (this == &contact)
    {
        return(*this);
    }

    this->name = contact.name;
    this->mobile = contact.mobile;
    this->office = contact.office;
    this->home = contact.home;
    this->email = contact.email;
    this->remark = contact.remark;
    this->type = contact.type;
    this->index = contact.index;

    return(*this);
}

bool
Contact::operator == (
                     const Contact& contact) const
{

    if (this == &contact)
    {
        return(true);
    }

    if (this->name != contact.name)
    {
        return(false);
    }

    if (this->mobile != contact.mobile)
    {
        return(false);
    }

    if (this->office != contact.office)
    {
        return(false);
    }

    if (this->home != contact.home)
    {
        return(false);
    }

    if (this->email != contact.email)
    {
        return(false);
    }

    if (this->remark != contact.remark)
    {
        return(false);
    }

    if (this->type != contact.type)
    {
        return(false);
    }

    if (this->index != contact.index)
    {
        return(false);
    }

    return(true);
}

QTextStream&
Contact::operator << (
                     QTextStream& textStream) const
{

    textStream << this->name ;
    textStream << this->mobile;
    textStream << this->office;
    textStream << this->home;
    textStream << this->email;
    textStream << this->remark;
    textStream << static_cast<int>(this->type) ;
    textStream << this->index;

    return(textStream);
}

QTextStream&
Contact::operator >> (
                     QTextStream& textStream)
{

    textStream >> this->name;
    textStream >> this->mobile;
    textStream >> this->office;
    textStream >> this->home;
    textStream >> this->email;
    textStream >> this->remark;

    int type = 0;
    textStream >> type;
    this->type = static_cast<Contact::Types>(type);

    textStream >> this->index;


    return(textStream);
}

const QString&
Contact::getName () const
{

    return(this->name);
}

const QString&
Contact::getMobile () const
{

    return(this->mobile);
}

const QString&
Contact::getOffice () const
{

    return(this->office);
}

const QString&
Contact::getHome () const
{

    return(this->home);
}

const QString&
Contact::getEmail () const
{

    return(this->email);
}

const QString&
Contact::getRemark () const
{

    return(this->remark);
}

Contact::Types
Contact::getType () const
{

    return(this->type);
}

int
Contact::getIndex () const
{

    return(this->index);
}

bool
Contact::setName (
                 const QString& name)
{

    this->name = name;
    return(true);
}

bool
Contact::setMobile (
                   const QString& mobile)
{

    this->mobile = mobile;
    return(true);
}

bool
Contact::setOffice (
                   const QString& office)
{

    this->office = office;
    return(true);
}

bool
Contact::setHome (
                 const QString& home)
{

    this->home = home;
    return(true);
}

bool
Contact::setEmail (
                  const QString& email)
{

    this->email = email;
    return(true);
}

bool
Contact::setRemark (
                   const QString& remark)
{

    this->remark = remark;
    return(true);
}

bool
Contact::setType (
                 Contact::Types type)
{

    this->type = type;
    return(true);
}

bool
Contact::setIndex (
                  int index)
{

    this->index = index;
    return(true);
}

const QStringList
Contact::String = QStringList ()
                  << "2 3 4 5 6"
                  << "3 4 5 6"
                  << "2"
                  << "3"
                  << "4"
                  << "5"
                  << "6";

const QStringList
Contact::Display = QStringList ()
                   << QT_TRANSLATE_NOOP("GLOBAL", "All")
                   << QT_TRANSLATE_NOOP("GLOBAL", "Local")
                   << QT_TRANSLATE_NOOP("GLOBAL", "Sim/Usim")
                   << QT_TRANSLATE_NOOP("GLOBAL", "Family")
                   << QT_TRANSLATE_NOOP("GLOBAL", "Friend")
                   << QT_TRANSLATE_NOOP("GLOBAL", "Colleague")
                   << QT_TRANSLATE_NOOP("GLOBAL", "Other");

const QStringList
Contact::ToolTip = QStringList ()
                   << QT_TRANSLATE_NOOP("GLOBAL", "All Contacts")
                   << QT_TRANSLATE_NOOP("GLOBAL", "Local Contacts")
                   << QT_TRANSLATE_NOOP("GLOBAL", "Sim/Usim Contacts")
                   << QT_TRANSLATE_NOOP("GLOBAL", "Family Contacts")
                   << QT_TRANSLATE_NOOP("GLOBAL", "Friend Contacts")
                   << QT_TRANSLATE_NOOP("GLOBAL", "Colleague Contacts")
                   << QT_TRANSLATE_NOOP("GLOBAL", "Other Contacts");

const QStringList
Contact::Header = QStringList ()
                  << QT_TRANSLATE_NOOP("GLOBAL", "Name")
                  << QT_TRANSLATE_NOOP("GLOBAL", "Mobile")
                  << QT_TRANSLATE_NOOP("GLOBAL", "Office")
                  << QT_TRANSLATE_NOOP("GLOBAL", "Home")
                  << QT_TRANSLATE_NOOP("GLOBAL", "Email")
                  << QT_TRANSLATE_NOOP("GLOBAL", "Remark")
                  << QT_TRANSLATE_NOOP("GLOBAL", "Type")
                  << QT_TRANSLATE_NOOP("GLOBAL", "Index");
