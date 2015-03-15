#include "DeamonMessage.h"
#include "DeamonPort.h"

DeamonMessage::DeamonMessage(QObject *parent) : QObject(parent)
{
}


bool DeamonMessage::sendMessage (Message message, bool save)
{
    if (true == save)
    {

    }
    else
    {

    }

    return (true);
}

Message DeamonMessage::getMessage ()
{

    return (Message("", "", "", Message::NONE));
}

bool DeamonMessage::deleteMessage ()
{

    return (true);
}

bool DeamonMessage::deleteAllMessage ()
{

    return (true);
}

bool DeamonMessage::dataAvailable ()
{

    return (true);
}

bool DeamonMessage::readStatus ()
{

    return (true);
}
