#include "Data.h"
#include "DeamonDatabase.h"
#include "Define.h"

#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

DeamonDatabase*
DeamonDatabase::singleton = NULL;

DeamonDatabase*
DeamonDatabase::getInstance ()
{

    if (NULL == singleton)
    {

        singleton = new DeamonDatabase();
    }

    return(singleton);
}

DeamonDatabase::DeamonDatabase (
                               QObject * parent)
: QObject(parent)
{

    sqlDatabase = QSqlDatabase::addDatabase(DATABASE_TYPE, DATABASE_CONNECTION);

    sqlDatabase.setHostName(DATABASE_HOST);

    sqlDatabase.setDatabaseName(DATABASE_NAME);
    sqlDatabase.setUserName(DATABASE_USER);
    sqlDatabase.setPassword(DATABASE_PASSWORD);


    if (false == sqlDatabase.open())
    {

        qDebug() <<" Unable to open sqlDatabase " << sqlDatabase.lastError();
    }

    QSqlQuery sqlQuery (sqlDatabase);

    if (false == (sqlQuery.exec(CREATE_TABLE_SETTING)))
    {

        qDebug () << "Setting table not created" << sqlQuery.lastError();
    }

    sqlQuery.clear ();

    if (false == (sqlQuery.exec (CREATE_TABLE_MESSAGE)))
    {

        qDebug () << "Message table not created" << sqlQuery.lastError ();
    }

    sqlQuery.clear ();

    if (false == (sqlQuery.exec (CREATE_TABLE_CONTACT)))
    {

        qDebug () << "Contacts table not created" << sqlQuery.lastError ();
    }

    sqlQuery.clear ();

    if (false == (sqlQuery.exec (CREATE_TABLE_CALLS)))
    {

        qDebug () << "Calls table not created" << sqlQuery.lastError ();
    }

    sqlQuery.clear ();

    if (false == (sqlQuery.exec (CREATE_TABLE_DATA)))
    {

        qDebug () << "Data table not created" << sqlQuery.lastError ();
    }
}

bool
DeamonDatabase::closeDatabase()
{

    sqlDatabase.commit();
    sqlDatabase.close();

    sqlDatabase = QSqlDatabase ();
    sqlDatabase.removeDatabase (DATABASE_CONNECTION);

    return(true);
}

Setting DeamonDatabase::toSetting (const QSqlQuery& sqlQuery)
{

    return (Setting(sqlQuery.value (0).toString (),
                    sqlQuery.value (1).toBool (),
                    sqlQuery.value (2).toInt ()));
}

QList <Setting>
DeamonDatabase::readSetting ()
{

    QList <Setting> result;
    QSqlQuery sqlQuery (this->sqlDatabase);

    if (false == sqlQuery.exec (SELECT_SETTING))
    {

        qDebug () << "SQL Unable to read Calls" << sqlQuery.lastError ();
    }
    else
    {

        while (true == sqlQuery.next ())
        {

            result << this->toSetting (sqlQuery);
        }
    }

    return (result);
}

Setting
DeamonDatabase::readSetting (
                            int index)
{

    QSqlQuery sqlQuery(this->sqlDatabase);

    sqlQuery.prepare (READ_SETTING);
    sqlQuery.bindValue (IDENTIFIER, index);

    bool result = sqlQuery.exec ();

    if (false == result)
    {

        qDebug () << "SQL Unable to read Call" << sqlQuery.lastError ();
        return (Setting());

    }
    else
    {

        return (Setting(this->toSetting (sqlQuery)));
    }
}

int
DeamonDatabase::addSetting (
                           const Setting& setting)
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    sqlQuery.prepare (INSERT_SETTING);
    sqlQuery.bindValue (VALUE, setting.getValue ());
    sqlQuery.bindValue (ENABLE, setting.getEnable ());

    if (false == sqlQuery.exec ())
    {
        qDebug () << "SQL Unable to add Setting" << sqlQuery.lastError ();
        return (-1);
    }

    sqlQuery.clear ();
    if (false == sqlQuery.exec (GET_INDEX))
    {
        qDebug () << "SQL Unable to add Setting" << sqlQuery.lastError ();
        return (-1);
    }
    else
    {
        sqlQuery.first ();
        return (sqlQuery.value (0).toInt ());
    }
}


bool DeamonDatabase::editSetting (
                                 const Setting& setting)
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    sqlQuery.prepare (EDIT_SETTING);
    sqlQuery.bindValue (VALUE, setting.getValue ());
    sqlQuery.bindValue (ENABLE, setting.getEnable ());
    sqlQuery.bindValue (IDENTIFIER, setting.getIndex ());

    bool result = sqlQuery.exec ();

    if (false == result)
    {

        qDebug () << "SQL Unable to edit Setting" << sqlQuery.lastError ();
    }

    return (result);
}

bool
DeamonDatabase::removeSetting (
                              int index)
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    sqlQuery.prepare (DELETE_SETTING);
    sqlQuery.bindValue (IDENTIFIER, index);

    bool result = sqlQuery.exec ();

    if (false == result)
    {

        qDebug () << "SQL Unable to remove Call" << sqlQuery.lastError ();
    }

    return (result);
}


bool
DeamonDatabase::removeSetting ()
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    bool result = sqlQuery.exec (DELETE_ALL_SETTING);

    if (false == result)
    {

        qDebug () << "SQL Unable to remove Calls" << sqlQuery.lastError ();
    }

    return (result);
}

Message
DeamonDatabase::toMessage (
                          const QSqlQuery& sqlQuery)
{

    return (Message (sqlQuery.value (0).toString (),
                     sqlQuery.value (1).toString (),
                     sqlQuery.value (2).toString (),
                     sqlQuery.value (3).toDateTime (),
                     static_cast<Message::Types> (sqlQuery.value (4).toInt ()),
                     sqlQuery.value (5).toInt (),
                     sqlQuery.value (6).toBool ()));
}

QList <Message>
DeamonDatabase::readMessage ()
{

    QList <Message> result;
    QSqlQuery sqlQuery(this->sqlDatabase);

    if (false == sqlQuery.exec (SELECT_MESSAGE))
    {

        qDebug () << "SQL Unable to read Messages" << sqlQuery.lastError ();
    }

    while (true == sqlQuery.next ())
    {

        result << this->toMessage (sqlQuery);
    }

    return (result);
}

Message
DeamonDatabase::readMessage (
                            int index)
{

    QSqlQuery sqlQuery(this->sqlDatabase);

    sqlQuery.prepare (READ_MESSAGE);
    sqlQuery.bindValue (IDENTIFIER, index);

    if (false == sqlQuery.exec ())
    {

        qDebug () << "SQL Unable to read Message" << sqlQuery.lastError ();
        return (Message ());

    }
    else
    {

        sqlQuery.first ();
        return (Message (this->toMessage (sqlQuery)));

    }
}

int
DeamonDatabase::addMessage (
                           const Message& message)
{

    QSqlQuery sqlQuery(this->sqlDatabase);

    sqlQuery.prepare(INSERT_MESSAGE);
    sqlQuery.bindValue (NUMBER, message.getNumber ());
    sqlQuery.bindValue (NAME, message.getName ());
    sqlQuery.bindValue (CONTENT, message.getContent ());
    sqlQuery.bindValue (TIME, message.getTime ());
    sqlQuery.bindValue (TYPE, static_cast<int>(message.getType ()));
    sqlQuery.bindValue (UNREAD, message.getUnread ());

    if (false == sqlQuery.exec ())
    {

        qDebug () << "SQL Unable to add Message" << sqlQuery.lastError ();
        return (-1);
    }

    sqlQuery.clear ();
    if (false == sqlQuery.exec (GET_INDEX))
    {

        qDebug () << "SQL Unable to add Message" << sqlQuery.lastError ();
        return (-1);

    }
    else
    {

        sqlQuery.first ();
        return (sqlQuery.value (0).toInt ());
    }
}

bool
DeamonDatabase::editMessage (
                            const Message& message)
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    sqlQuery.prepare (EDIT_MESSAGE);
    sqlQuery.bindValue (NUMBER, message.getNumber ());
    sqlQuery.bindValue (NAME, message.getName ());
    sqlQuery.bindValue (CONTENT, message.getContent ());
    sqlQuery.bindValue (TIME, message.getTime ());
    sqlQuery.bindValue (TYPE, static_cast<int>(message.getType ()));
    sqlQuery.bindValue (UNREAD, message.getUnread ());
    sqlQuery.bindValue (IDENTIFIER, message.getIndex ());

    bool result = sqlQuery.exec ();

    if (false == result)
    {

        qDebug () << "SQL Unable to edit Message" << sqlQuery.lastError ();
    }

    return (result);
}

bool
DeamonDatabase::removeMessage (
                              int index)
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    sqlQuery.prepare (DELETE_MESSAGE);
    sqlQuery.bindValue (IDENTIFIER, index);

    bool result = sqlQuery.exec ();

    if (false == result)
    {
        qDebug () << "SQL Unable to Delete Message" << sqlQuery.lastError ();
    }

    return(result);
}

bool
DeamonDatabase::removeMessage ()
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    bool result = sqlQuery.exec (DELETE_ALL_MESSAGE);

    if (false == result)
    {

        qDebug () << "SQL Unable to remove All Message" << sqlQuery.lastError ();
    }

    return (result);
}

Contact
DeamonDatabase::toContact (
                          const QSqlQuery& sqlQuery)
{

    return (Contact (sqlQuery.value (0).toString (),
                     sqlQuery.value (1).toString (),
                     sqlQuery.value (2).toString (),
                     sqlQuery.value (3).toString (),
                     sqlQuery.value (4).toString (),
                     sqlQuery.value (5).toString (),
                     static_cast<Contact::Types>(sqlQuery.value (6).toInt ()),
                     sqlQuery.value (7).toInt ()));
}

QList<Contact>
DeamonDatabase::readContact ()
{

    QList<Contact> result;

    QSqlQuery sqlQuery (this->sqlDatabase);

    if (false == sqlQuery.exec (SELECT_CONTACT))
    {

        qDebug () << "SQL Unable to read Contacts" << sqlQuery.lastError ();
    }
    else
    {

        while (true == sqlQuery.next ())
        {

            result << this->toContact (sqlQuery);
        }
    }

    return (result);
}

Contact
DeamonDatabase::readContact (int index)
{

    QSqlQuery sqlQuery(this->sqlDatabase);

    sqlQuery.prepare (READ_CONTACT);
    sqlQuery.bindValue (IDENTIFIER, index);

    if (false == sqlQuery.exec ())
    {

        qDebug () << "SQL Unable to read Contact" << sqlQuery.lastError ();
        return (Contact ());

    }
    else
    {

        sqlQuery.first ();
        return (Contact (this->toContact (sqlQuery)));
    }

}

int
DeamonDatabase::addContact (
                           const Contact& contact)
{

    QSqlQuery sqlQuery(this->sqlDatabase);

    sqlQuery.prepare(INSERT_CONTACT);
    sqlQuery.bindValue (NAME, contact.getName ());
    sqlQuery.bindValue (MOBILE, contact.getMobile ());
    sqlQuery.bindValue (OFFICE, contact.getOffice ());
    sqlQuery.bindValue (HOME, contact.getHome ());
    sqlQuery.bindValue (EMAIL, contact.getEmail ());
    sqlQuery.bindValue (REMARK, contact.getRemark ());
    sqlQuery.bindValue (TYPE, static_cast<int> (contact.getType ()));

    if (false == sqlQuery.exec ())
    {

        qDebug () << "SQL Unable to add Contact" << sqlQuery.lastError ();
        return (-1);
    }

    sqlQuery.clear ();
    if (false == sqlQuery.exec (GET_INDEX))
    {

        qDebug () << "SQL Unable to add Contact" << sqlQuery.lastError ();
        return (-1);

    }
    else
    {

        sqlQuery.first ();
        return (sqlQuery.value (0).toInt ());
    }
}

bool
DeamonDatabase::editContact (const Contact& contact)
{

    QSqlQuery sqlQuery(this->sqlDatabase);

    sqlQuery.prepare(EDIT_CONTACT);
    sqlQuery.bindValue (NAME, contact.getName ());
    sqlQuery.bindValue (MOBILE, contact.getMobile ());
    sqlQuery.bindValue (OFFICE, contact.getOffice ());
    sqlQuery.bindValue (HOME, contact.getHome ());
    sqlQuery.bindValue (EMAIL, contact.getEmail ());
    sqlQuery.bindValue (REMARK, contact.getRemark ());
    sqlQuery.bindValue (TYPE, static_cast<int> (contact.getType ()));
    sqlQuery.bindValue (IDENTIFIER, contact.getIndex ());

    bool result = sqlQuery.exec ();

    if (false == result)
    {

        qDebug () << "SQL Unable to edit Contact" << sqlQuery.lastError ();
    }

    return (result);
}

bool
DeamonDatabase::removeContact (int index)
{

    QSqlQuery sqlQuery(this->sqlDatabase);

    sqlQuery.prepare (DELETE_CONTACT);
    sqlQuery.bindValue (IDENTIFIER, index);

    bool result = sqlQuery.exec ();

    if (false == result)
    {

        qDebug () << "SQL Unable to Delete Contact" << sqlQuery.lastError ();
    }

    return(result);
}

bool
DeamonDatabase::removeContact ()
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    bool result = sqlQuery.exec (DELETE_ALL_CONTACT);

    if (false == result)
    {

        qDebug () << "SQL Unable to remove All Contacts" << sqlQuery.lastError ();
    }

    return (result);
}

Call
DeamonDatabase::toCall (
                       const QSqlQuery& sqlQuery)
{

    return (Call(sqlQuery.value (0).toString (),
                 sqlQuery.value (1).toString (),
                 sqlQuery.value (2).toDateTime (),
                 sqlQuery.value (3).toTime (),
                 static_cast<Call::Types>(sqlQuery.value (4).toInt ()),
                 sqlQuery.value (5).toInt (),
                 sqlQuery.value (6).toBool ()));
}

QList<Call>
DeamonDatabase::readCall ()
{

    QList <Call> result;
    QSqlQuery sqlQuery (this->sqlDatabase);

    if (false == sqlQuery.exec (SELECT_CALL))
    {

        qDebug () << "SQL Unable to read Calls" << sqlQuery.lastError ();
    }
    else
    {

        while (true == sqlQuery.next ())
        {

            result << this->toCall(sqlQuery);
        }
    }

    return (result);
}

Call
DeamonDatabase::readCall (
                         int index)
{

    QSqlQuery sqlQuery(this->sqlDatabase);

    sqlQuery.prepare (READ_CALL);
    sqlQuery.bindValue (IDENTIFIER, index);
    bool result = sqlQuery.exec ();

    if (false == result)
    {

        qDebug () << "SQL Unable to read Call" << sqlQuery.lastError ();
        return (Call());

    }
    else
    {

        return (Call(toCall(sqlQuery)));
    }
}

int
DeamonDatabase::addCall (
                        const Call& call)
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    sqlQuery.prepare (INSERT_CALL);
    sqlQuery.bindValue (NUMBER, call.getNumber ());
    sqlQuery.bindValue (NAME, call.getName ());
    sqlQuery.bindValue (START, call.getStartTime ());
    sqlQuery.bindValue (DURATION, call.getDuration ());
    sqlQuery.bindValue (TYPE, call.getType ());
    sqlQuery.bindValue (UNREAD, call.getUnread ());

    if (false == sqlQuery.exec ())
    {

        qDebug () << "SQL Unable to add Call" << sqlQuery.lastError ();
        return (-1);
    }

    sqlQuery.clear ();
    if (false == sqlQuery.exec (GET_INDEX))
    {

        qDebug () << "SQL Unable to add Call" << sqlQuery.lastError ();
        return (-1);

    }
    else
    {

        sqlQuery.first ();
        return (sqlQuery.value (0).toInt ());
    }
}

bool
DeamonDatabase::editCall(
                        const Call& call)
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    sqlQuery.prepare (EDIT_CALL);
    sqlQuery.bindValue (NUMBER, call.getNumber ());
    sqlQuery.bindValue (NAME, call.getName ());
    sqlQuery.bindValue (START, call.getStartTime ());
    sqlQuery.bindValue (DURATION, call.getDuration ());
    sqlQuery.bindValue (TYPE, call.getType ());
    sqlQuery.bindValue (IDENTIFIER, call.getIndex ());
    sqlQuery.bindValue (UNREAD, call.getUnread ());

    bool result = sqlQuery.exec ();
    if (false == result)
    {

        qDebug () << "SQL Unable to edit Call" << sqlQuery.lastError ();
    }

    return (result);
}

bool
DeamonDatabase::removeCall (
                           int index)
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    sqlQuery.prepare (DELETE_CALL);
    sqlQuery.bindValue (IDENTIFIER, index);

    bool result = sqlQuery.exec ();

    if (false == result)
    {

        qDebug () << "SQL Unable to remove Call" << sqlQuery.lastError ();
    }

    return (result);
}


bool
DeamonDatabase::removeCall ()
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    bool result = sqlQuery.exec (DELETE_ALL_CALL);

    if (false == result)
    {

        qDebug () << "SQL Unable to remove All Calls" << sqlQuery.lastError ();
    }

    return (result);
}

Data
DeamonDatabase::toData (
                       const QSqlQuery& sqlQuery)
{

    return (Data(sqlQuery.value (0).toDouble (),
                 sqlQuery.value (1).toDouble (),
                 sqlQuery.value (2).toDateTime (),
                 sqlQuery.value (3).toInt()));
}

QList<Data>
DeamonDatabase::readData ()
{

    QList<Data> result;
    QSqlQuery sqlQuery (this->sqlDatabase);

    sqlQuery.prepare (SELECT_DATA);

    if (false == sqlQuery.exec ())
    {

        qDebug () << "SQL Unable to read Data" << sqlQuery.lastError ();

    }
    else
    {

        while (true == sqlQuery.next ())
        {

            result << this->toData (sqlQuery);
        }
    }

    return (result);
}

Data
DeamonDatabase::readData (
                         int index)
{

    QSqlQuery sqlQuery(this->sqlDatabase);

    sqlQuery.prepare (READ_DATA);
    sqlQuery.bindValue (IDENTIFIER, index);
    bool result = sqlQuery.exec ();

    if (false == result)
    {

        qDebug () << "SQL Unable to read Data" << sqlQuery.lastError ();
        return (Data ());

    }
    else
    {

        return (Data (this->toData (sqlQuery)));
    }
}

int
DeamonDatabase::addData(
                       const Data& data)
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    sqlQuery.prepare (INSERT_DATA);
    sqlQuery.bindValue (DOWNLOAD, data.getDownload ());
    sqlQuery.bindValue (UPLOAD, data.getUpload ());
    sqlQuery.bindValue (START, data.getStart ());

    if (false == sqlQuery.exec ())
    {

        qDebug () << "SQL Unable to add Data" << sqlQuery.lastError ();
        return (-1);
    }

    sqlQuery.clear ();
    if (false == sqlQuery.exec (GET_INDEX))
    {

        qDebug () << "SQL Unable to add Data" << sqlQuery.lastError ();
        return (-1);

    }
    else
    {

        sqlQuery.first ();
        return (sqlQuery.value (0).toInt ());
    }
}

bool
DeamonDatabase::editData(
                        const Data& data)
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    sqlQuery.prepare (EDIT_DATA);
    sqlQuery.bindValue (DOWNLOAD, data.getDownload ());
    sqlQuery.bindValue (UPLOAD, data.getUpload ());
    sqlQuery.bindValue (START, data.getStart ());
    sqlQuery.bindValue (IDENTIFIER, data.getIndex ());

    bool result = sqlQuery.exec ();

    if (false == result)
    {

        qDebug () << "SQL Unable to edit Data" << sqlQuery.lastError ();
    }

    return (result);
}

bool
DeamonDatabase::removeData (
                           int index)
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    sqlQuery.prepare (DELETE_DATA);
    sqlQuery.bindValue (IDENTIFIER, index);

    bool result = sqlQuery.exec ();

    if (false == result)
    {

        qDebug () << "SQL Unable to remove Data" << sqlQuery.lastError ();
    }

    return (result);
}

bool
DeamonDatabase::removeData ()
{

    bool result = false;

    QSqlQuery sqlQuery (this->sqlDatabase);

    result = sqlQuery.exec (DELETE_ALL_DATA);

    if (false == result)
    {

        qDebug () << "SQL Unable to remove All Datas" << sqlQuery.lastError ();
    }

    return (result);
}

Data
DeamonDatabase::checkData ()
{

    QSqlQuery sqlQuery (this->sqlDatabase);

    bool result = sqlQuery.exec (CHECK_DATA);
    Data data;

    if (false == result)
    {

        qDebug () << "SQL Unable to check data" << sqlQuery.lastError ();
    }
    else
    {

        while ( true == sqlQuery.next ())
        {

            data.addUpload (sqlQuery.value (0).toDouble ());
            data.addDownload (sqlQuery.value (1).toDouble ());
        }
    }

    return (data);
}
