#include "Call.h"
#include "Define.h"

Call::Call ()
{
    this->type = Call::Dialed;
    this->index = -1;
    this->unread = false;
}

Call::Call (
           const QString& number,
           const QString& name,
           const QDateTime& startTime,
           const QTime& duration,
           Call::Types type,
           int index,
           bool unread)
: number (number),
name (name),
startTime (startTime),
duration (duration),
type (type),
index (index),
unread (unread)
{
}

Call::Call (
           const Call& call)
: number (call.number),
name (call.name),
startTime (call.startTime),
duration (call.duration),
type (call.type),
index (call.index),
unread (call.unread)
{
}

Call&
Call::operator = (
                 const Call& call) {

    if (this == &call) return (* this);

    this->number = call.number;
    this->name = call.name;
    this->startTime = call.startTime;
    this->duration = call.duration;
    this->type = call.type;
    this->index = call.index;
    this->unread = call.unread;

    return (* this);
}

bool
Call::operator == (
                  const Call& call) const
{

    if (this == &call)
    {
        return (true);
    }

    if (this->number != call.number)
    {
        return (false);
    }

    if (this->name != call.name)
    {
        return (false);
    }

    if (this->startTime != call.startTime)
    {
        return (false);
    }

    if (this->duration != call.duration)
    {
        return (false);
    }

    if (this->type != call.type)
    {
        return (false);
    }

    if (this->index != call.index)
    {
        return (false);
    }

    if (this->unread != call.unread)
    {
        return (false);
    }

    return (true);
}

QTextStream&
Call::operator << (
                  QTextStream& textStream) const
{

    textStream << this->number;
    textStream << this->name;
    textStream << this->startTime.toString (DATETIME_FORMAT);
    textStream << this->duration.toString (TIME_FORMAT);
    textStream << static_cast<int> (this->type);
    textStream << this->index;
    textStream << this->unread;

    return (textStream);
}

QTextStream&
Call::operator >> (
                  QTextStream& textStream)
{

    textStream >> this->number;
    textStream >> this->name;

    QString string;
    textStream >> string;
    this->startTime.fromString (DATETIME_FORMAT);

    textStream >> string;
    this->duration.fromString (TIME_FORMAT);

    int integer = 0;
    textStream >> integer;
    this->type = static_cast<Call::Types> (integer);

    textStream >> index;

    textStream >> integer;
    this->unread = (1 == integer);

    return (textStream);
}

const
QString& Call::getNumber() const
{

    return (this->number);
}

const
QString& Call::getName() const
{

    return (this->name);
}

const
QDateTime& Call::getStartTime() const
{

    return (this->startTime);
}

const
QTime& Call::getDuration () const
{

    return (this->duration);
}

Call::Types
Call::getType () const
{

    return (this->type);
}

int
Call::getIndex () const
{

    return (this->index);
}

bool Call::getUnread () const
{

    return (this->unread);
}


bool
Call::setNumber (
                const QString& number)
{

    this->number = number;
    return (true);
}

bool
Call::setName (
              const QString& name)
{

    this->name = name;
    return (true);
}

bool
Call::setStartTime (
                   const QDateTime& startTime)
{

    this->startTime = startTime;
    return (true);
}

bool
Call::setDuration (
                  const QTime& endTime)
{

    this->duration = endTime;
    return (true);
}

bool
Call::setType (
              Call::Types type)
{

    this->type = type;
    return (true);
}

bool
Call::setIndex (
               int index)
{

    this->index = index;
    return (true);
}

bool
Call::setUnread (
                bool unread)
{

    this->unread = unread;
    return (true);
}

const QStringList
Call::String = QStringList ()
               << "1 2 3"
               << "1"
               << "2"
               << "3";

const QStringList
Call::Display = QStringList ()
                << QT_TRANSLATE_NOOP("GLOBAL", "All")
                << QT_TRANSLATE_NOOP("GLOBAL", "Answered")
                << QT_TRANSLATE_NOOP("GLOBAL", "Missed")
                << QT_TRANSLATE_NOOP("GLOBAL", "Dialed");

const QStringList
Call::ToolTip = QStringList ()
                << QT_TRANSLATE_NOOP("GLOBAL", "All Calls")
                << QT_TRANSLATE_NOOP("GLOBAL", "Answered Calls")
                << QT_TRANSLATE_NOOP("GLOBAL", "Missed Calls")
                << QT_TRANSLATE_NOOP("GLOBAL", "Dialed Calls");

const QStringList
Call::Header = QStringList ()
               << QT_TRANSLATE_NOOP("GLOBAL", "Number")
               << QT_TRANSLATE_NOOP("GLOBAL", "Name")
               << QT_TRANSLATE_NOOP("GLOBAL", "Start Time")
               << QT_TRANSLATE_NOOP("GLOBAL", "Duration")
               << QT_TRANSLATE_NOOP("GLOBAL", "Type")
               << QT_TRANSLATE_NOOP("GLOBAL", "Index")
               << QT_TRANSLATE_NOOP("GLOBAL", "Unread");
