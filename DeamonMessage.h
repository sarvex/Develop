#ifndef DEAMONMESSAGE_H
#define DEAMONMESSAGE_H

#include <QObject>

#include "Message.h"

class DeamonMessage : public QObject
{
    Q_OBJECT
public:
    explicit DeamonMessage(QObject *parent = 0);

    bool sendMessage (Message message, bool save);

    Message getMessage ();

    bool deleteMessage ();
    bool deleteAllMessage ();

    signals:

public slots:
    bool dataAvailable ();
    bool readStatus ();

protected:

private:

};

#endif // DEAMONMESSAGE_H
