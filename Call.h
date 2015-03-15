#ifndef CALL
#define CALL

#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QTime>

class Call
{
public:

    static const QStringList String;
    static const QStringList Display;
    static const QStringList Header;
    static const QStringList ToolTip;

    enum Fields
    {
        Number,
        Name,
        StartTime,
        Duration,
        Type,
        Index,
        Unread,
        FieldCount
    };

    enum Types
    {
        All,
        Answered,
        Missed,
        Dialed,
        TypeCount
    };

    Call ();
    Call (const Call& call);
    Call (const QString& number,
          const QString& name,
          const QDateTime& startTime,
          const QTime& duration,
          Types type,
          int index = -1,
          bool unread = true);

    Call& operator = (const Call& call);
    bool operator == (const Call& call) const;

    QTextStream& operator << (QTextStream& textStream) const;
    QTextStream& operator >> (QTextStream& textStream);

    const QString& getNumber() const;
    const QString& getName() const;
    const QDateTime& getStartTime() const;
    const QTime& getDuration() const;
    Types getType () const;
    int getIndex() const;
    bool getUnread () const;

    bool setNumber (const QString& number);
    bool setName (const QString& name);
    bool setStartTime (const QDateTime& startTime);
    bool setDuration (const QTime& duration);
    bool setType (Types type);
    bool setIndex (int index);
    bool setUnread (bool unread);

protected:

private:
    QString number;
    QString name;
    QDateTime startTime;
    QTime duration;
    Types type;
    int index;
    bool unread;
};

#endif // CALL
