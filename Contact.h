#ifndef CONTACT
#define CONTACT

#include <QString>
#include <QStringList>
#include <QTextStream>

class Contact
{
public:
    static const QStringList String;
    static const QStringList Display;
    static const QStringList Header;
    static const QStringList ToolTip;

    enum Types
    {
        All,
        Local,
        Sim,
        Family,
        Friend,
        Colleague,
        Other,
        TypeCount
    };

    enum Fields
    {
        Name,
        Mobile,
        Office,
        Home,
        Email,
        Remark,
        Type,
        Index,
        FieldCount
    };

    Contact ();
    Contact (const Contact& contact);
    Contact (const QString& name,
             const QString& mobile,
             const QString& office,
             const QString& home,
             const QString& email,
             const QString& remark,
             Types type,
             int index = -1);

    Contact& operator = (const Contact& contact);
    bool operator == (const Contact& contact) const;

    QTextStream& operator << (QTextStream& textStream) const;
    QTextStream& operator >> (QTextStream& textStream);

    const QString& getName () const;
    bool setName (const QString& name);

    const QString& getMobile() const;
    bool setMobile (const QString& mobile);

    const QString& getOffice () const;
    bool setOffice (const QString& office);

    const QString& getHome () const;
    bool setHome (const QString& home);

    const QString& getEmail () const;
    bool setEmail (const QString& email);

    const QString& getRemark () const;
    bool setRemark (const QString& remark);

    Types getType () const;
    bool setType (Types type);

    int getIndex () const;
    bool setIndex (int index);

protected:

private:
    QString name;
    QString mobile;
    QString office;
    QString home;
    QString email;
    QString remark;
    Types type;
    int index;
};

#endif // CONTACT
