#include "Define.h"
#include "Message.h"

Message::Message () {
    this->type = Message::Draft;
    this->index = -1;
    this->unread = true;
}

Message::Message(const QString& number,
                 const QString& name,
                 const QString& content,
                 const QDateTime& time,
                 Message::Types type,
                 int index,
                 bool unread)
    : number (number),
      name(name),
      content (content),
      time (time),
      type (type),
      index(index),
      unread (unread){
}

Message::Message (
        const Message& message)
    : number (message.number),
      name (message.name),
      content (message.content),
      time (message.time),
      type (message.type),
      index (message.index),
      unread (message.unread) {
}

Message&
Message::operator = (
        const Message& message) {
    if (this == &message) return *this;

    this->number = message.number;
    this->name = message.name;
    this->content = message.content;
    this->time = message.time;
    this->type = message.type;
    this->index = message.index;
    this->unread = message.unread;

    return *this;
}

bool
Message::operator == (
        const Message& message) const {

    if (this == &message) return true;

    if (this->number != message.number) return false;
    if (this->name != message.name) return false;
    if (this->content != message.content) return false;
    if (this->time != message.time) return false;
    if (this->type != message.type) return false;
    if (this->index != message.index) return false;
    if (this->unread != message.unread) return false;

    return true;
}

QTextStream&
Message::operator << (
        QTextStream& textStream) const {

    textStream << this->number;
    textStream << this->name;
    textStream << this->content;
    textStream << this->time.toString (DATETIME_FORMAT);
    textStream << this->type;
    textStream << this->index;
    textStream << static_cast<int>(this->unread);

    return textStream;
}

QTextStream&
Message::operator >> (
        QTextStream& textStream) {

    textStream >> this->number;
    textStream >> this->name;
    textStream >> this->content;

    QString time;
    textStream >> time;
    this->time.fromString (time, DATETIME_FORMAT);

    int integer = 0;
    textStream >> integer;
    this->type = static_cast<Message::Types> (integer);

    textStream >> this->index;

    textStream >> integer;
    this->unread = (1 == integer);

    return textStream;
}

const QString&
Message::getName () const {

    return this->name;
}

const QString&
Message::getNumber ()  const {

    return this->number;
}

const QString&
Message::getContent ()  const {

    return this->content;
}

const QDateTime&
Message::getTime ()  const {

    return this->time;
}

Message::Types
Message::getType () const {

    return this->type;
}

int
Message::getIndex () const {

    return this->index;
}

bool
Message::getUnread () const {

    return this->unread;
}


bool
Message::setName (
        const QString& name) {

    this->name = name;
    return true;
}

bool
Message::setNumber (
        const QString& number) {

    this->number = number;
    return true;
}

bool
Message::setContent (
        const QString& content){

    this->content = content;
    return true;
}

bool
Message::setTime (
        const QDateTime& time) {

    this->time = time;
    return true;
}

bool
Message::setType (
        Message::Types type) {

    this->type = type;
    return true;
}

bool
Message::setIndex (
        int index) {

    this->index = index;
    return true;
}

bool
Message::setUnread (bool unread) {

    this->unread = unread;
    return true;
}

const QStringList
Message::String = QStringList ()
                  << "3 4 5 6 7 8 9 10"
                  << "3 4 5 6 7 8"
                  << "9 10"
                  << "3"
                  << "4"
                  << "5"
                  << "6"
                  << "7"
                  << "8"
                  << "9"
                  << "10";

const QStringList
Message::Display = QStringList ()
                   << "All"
                   << "Local"
                   << "Sim/Usim"
                   << "Inbox"
                   << "Outbox"
                   << "Draft"
                   << "Important"
                   << "Deleted"
                   << "Report"
                   << "Inbox"
                   << "Outbox";

const QStringList
Message::ToolTip = QStringList ()
                   << "All Messages"
                   << "All Local Messages"
                   << "All Sim Messages"
                   << "Inbox Messages"
                   << "Outbox Messages"
                   << "Draft Messages"
                   << "Important Messages"
                   << "Deleted Messages"
                   << "Report Messages"
                   << "SIM/USIM Inbox Messages"
                   << "SIM/USIM Outbox Messages";

const QStringList
Message::Header = QStringList ()
                  << "Number"
                  << "Name/Number"
                  << "Content"
                  << "Time"
                  << "Type"
                  << "Index"
                  << "Read";
