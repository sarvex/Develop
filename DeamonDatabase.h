#ifndef DEAMON_DATABASE
#define DEAMON_DATABASE

#include "Contact.h"
#include "Call.h"
#include "Data.h"
#include "DataWidget.h"
#include "Message.h"
#include "Setting.h"

#include <QList>
#include <QObject>
#include <QSqlDatabase>

class DeamonDatabase
: public QObject
{
    Q_OBJECT

public:
    static DeamonDatabase* getInstance ();

    bool initilize();
    bool closeDatabase();

    QList <Setting> readSetting ();
    Setting readSetting (int index);
    int addSetting (const Setting& setting);
    bool editSetting (const Setting& setting);
    bool removeSetting (int index);
    bool removeSetting ();

    QList <Message> readMessage ();
    Message readMessage (int index);
    int addMessage (const Message& message);
    bool editMessage (const Message& message);
    bool removeMessage (int index);
    bool removeMessage ();

    QList <Contact> readContact ();
    Contact readContact (int index);
    int addContact (const Contact& contact);
    bool editContact (const Contact& contact);
    bool removeContact (int index);
    bool removeContact ();

    QList <Call> readCall ();
    Call readCall (int index);
    int addCall (const Call& call);
    bool editCall (const Call& call);
    bool removeCall (int index);
    bool removeCall ();

    QList <Data> readData ();
    Data readData (int index);
    int addData (const Data& data);
    bool editData (const Data& data);
    bool removeData (int index);
    bool removeData ();
    Data checkData ();

    signals:

public slots:

protected:

private:
    explicit DeamonDatabase(QObject *parent = 0);
    static DeamonDatabase* singleton;

    Message toMessage (const QSqlQuery& sqlQuery);
    Contact toContact (const QSqlQuery& sqlQuery);
    Call toCall (const QSqlQuery& sqlQuery);
    Data toData (const QSqlQuery& sqlQuery);
    Setting toSetting (const QSqlQuery& sqlQuery);

    QSqlDatabase sqlDatabase;
};

#endif // DEAMON_DATABASE
