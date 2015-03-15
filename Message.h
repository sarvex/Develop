#ifndef MESSAGE
#define MESSAGE

#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QTextStream>

class Message {
public:
    static const QStringList String;
    static const QStringList Display;
    static const QStringList Header;
    static const QStringList ToolTip;

    enum Fields {
        Number,
        Name,
        Content,
        Time,
        Type,
        Index,
        Unread,
        FieldCount
    };

    enum Types {
        All,
        Local,
        Sim,
        Inbox,
        Outbox,
        Draft,
        Important,
        Deleted,
        Report,
        SimInbox,
        SimOutbox,
        TypeCount
    };


    Message ();
    Message (const Message& message);
    Message (const QString& number,
             const QString& name,
             const QString& content,
             const QDateTime& time,
             Message::Types type,
             int index = -1,
             bool unread = true);

    Message& operator = (const Message& message);
    bool operator == (const Message& message) const;

    QTextStream& operator << (QTextStream& textStream) const;
    QTextStream& operator >> (QTextStream& textStream);

    const QString& getNumber () const;
    bool setNumber (const QString& number);

    const QString& getName () const;
    bool setName (const QString& name);

    const QString& getContent () const;
    bool setContent (const QString& content);

    const QDateTime& getTime () const;
    bool setTime (const QDateTime& time);

    Types getType () const;
    bool setType (Types type);

    int getIndex () const;
    bool setIndex (int index);

    bool getUnread () const;
    bool setUnread (bool unread);

protected:

private:
    QString number;
    QString name;
    QString content;

    QDateTime time;

    Types type;
    int index;

    bool unread;
};

#endif // MESSAGE
