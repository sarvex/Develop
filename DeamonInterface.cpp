#include "SerialEnumerator.h"

#include "At.h"
#include "DeamonInterface.h"
#include "DeamonSetting.h"
#include "Define.h"
#include "Message.h"

#include <QDebug>
#include <QStringList>
#include <QTime>

DeamonInterface*
DeamonInterface::singleton = NULL;

DeamonInterface*
DeamonInterface::getInstance ()
{

    if (NULL == singleton)
    {

        singleton = new DeamonInterface();
    }

    return(singleton);
}

DeamonInterface::DeamonInterface (
                                 QObject *parent)
: QThread(parent),
port (NULL),
running (false),
plugged (false),
incomingCall (false),
outgoingCall (false)
{

    this->mutex = new QMutex();

    this->write (At::setEchoOn);
    this->write (At::pinCodeStatus);

    this->write (At::setContactSim);
    this->write (At::setFullModemFunctionality);
    this->write (At::setMessageModeText);
    this->write (At::setAccessPointName);
    this->write (At::setNetworkModeGsm);
    this->write (At::setMessageApplication);

    this->write (At::signalStrength);
    this->write (At::setCellIdentifier);
    this->write (At::networkName);

    this->systemInformationTimer = new QTimer(this);
    this->systemInformationTimer->start(SIGNAL_STRENGTH_TIMEOUT);

    QObject::connect (systemInformationTimer,
                      SIGNAL(timeout()),
                      SLOT(onSystemUpdateTimer()));
}

bool
DeamonInterface::configure ()
{

    this->write (At::setContactSim);
    this->write (At::setFullModemFunctionality);
    this->write (At::setMessageModeText);
    this->write (At::setAccessPointName);
    this->write (At::setNetworkModeGsm);
    this->write (At::setMessageApplication);

    return (true);
}

bool
DeamonInterface::onSystemUpdateTimer ()
{

    this->write (At::signalStrength);
    this->write (At::cellIdentifier);
    this->write (At::networkName);

    return (true);
}

void
DeamonInterface::run ()
{
    this->running = true;

    while (true == this->running)
    {

        if ( true == this->checkPort ())
        {

            if ( false == this->openPort ())
            {

                continue;
            }


            this->read ();

            this->msleep (THREAD_SLEEP_TIMEOUT);

            this->write ();

            this->msleep (THREAD_SLEEP_TIMEOUT);

        }

        this->msleep (THREAD_SLEEP_TIMEOUT);

    }
}

bool
DeamonInterface::checkPort ()
{

    bool result = false;

    QList<PortInformation> ports = SerialEnumerator::getPorts();

    foreach (PortInformation port, ports)
    {

        if (VENDOR_ID == port.vendorID)
        {

            if (PRODUCT_ID == port.productID)
            {

                if (port.friendName.contains (INTERFACE_PORT_NAME))
                {

                    this->portName = port.portName.remove ("\\\\.\\");
                    emit interfacePortUpdate (this->portName);

                    result = true;
                    break;
                }
            }
        }
    }

    if (false == result)
    {
        this->portName = "";
    }

    if (this->plugged != result)
    {

        this->plugged = result;
        emit modemPlugged (result);
    }

    return (result);
}

bool
DeamonInterface::openPort ()
{

    bool result = false;

    if (true == this->portName.isEmpty ())
    {

        if ( NULL != this->port)
        {

            delete this->port;
            this->port = NULL;
        }

        result = false;

    }
    else
    {
        if ( NULL == this->port)
        {

            this->port = new SerialPort(portName, SerialPort::Polling);

            if ( true == this->port->open (QIODevice::ReadWrite|QIODevice::Unbuffered) )
            {

                qDebug () << "Successfully opened Interface Port" << this->portName;
                this->port->setBaudRate (BAUD115200);
                this->port->setDataBits (DATA_8);
                this->port->setParity (PAR_NONE);
                this->port->setStopBits (STOP_1);
                this->port->setFlowControl (FLOW_HARDWARE);
                this->port->setTimeout (PORT_TIMEOUT);

                result = true;

            }
            else
            {

                result = false;
            }

        }
        else
        {

            result = true;
        }
    }

    return (result);
}

bool
DeamonInterface::write(
                      QString value)
{

    this->mutex->lock ();

    this->queue.enqueue (value);

    this->mutex->unlock ();

    return (true);
}

bool
DeamonInterface::enqueue (
                         QString value)
{

    this->mutex->lock ();

    this->waited.enqueue (value);

    this->mutex->unlock ();

    return (true);
}

bool
DeamonInterface::write ()
{

    bool result = false;

    this->mutex->lock();

    if ( false == this->queue.isEmpty ())
    {

        QString data = this->queue.dequeue ().append (At::end);

        int written = this->port->write (data.toAscii (), data.length ());

        qDebug() << "Write to AT port :"
        << QTime::currentTime().toString("hh:mm:ss.zzz")
        << "[" << written << "]" << data;

        this->port->flush ();

        result = true;
    }

    if ( false == this->waited.isEmpty ())
    {

        QString data = this->waited.dequeue ().append (At::end);

        int written = this->port->write (data.toAscii (), data.length ());

        qDebug() << "Write Wait to AT port :"
        << QTime::currentTime().toString("hh:mm:ss.zzz")
        << "[" << written << "]" << data;

        this->port->flush ();

        this->msleep (2000);

        result = true;
    }


    this->mutex->unlock();

    return (result);
}

bool
DeamonInterface::read ()
{

    int size = this->port->bytesAvailable();

    if (0 == size) return (false);

    QByteArray data;
    data.resize (size);

    int read = this->port->read(data.data (), size);
    data.resize (read);

    data.replace ("\0", 1, " ", 1);


    qDebug() << "Read From AT port :"
    << QTime::currentTime().toString("hh:mm:ss.zzz")
    << "[" << read << "]" << data;

    return (this->parse (data));
}

bool
DeamonInterface::parse (
                       const QByteArray& value)
{

    bool result = false;

    QString response (value);

    response.remove (At::error);
    response.remove (At::dormant0);
    response.remove (At::dormant1);

    response.remove (At::setEchoOn);
    response.remove (At::setContactSim);
    response.remove (At::setFullModemFunctionality);
    response.remove (At::setMessageModeText);
    response.remove (At::setNetworkModeGsm);
    response.remove (At::enableDeliveryAcknowledgement);
    response.remove (At::disableDeliveryAcknowledgement);
    response.remove (At::setMessageApplication);
    response.remove (At::setAccessPointName);

    response = response.trimmed ();

    QStringList list = response.split("\nOK", QString::SkipEmptyParts);

    foreach (QString item, list )
    {
        item = item.trimmed ();

        qDebug() << "Parsing Item : " << item;

        if (item.contains (At::ringBack))
        {

            item.remove (At::ringBack);
            qDebug () << "Call : Outgoing Ring";
            emit ringBackUpdate ();

        }
        else if (item.contains (At::stopRing))
        {

            item.remove (At::stopRing);
            qDebug () << "Call : Outgoing End";
            emit stopRingUpdate ();

        }
        else if (item.contains (At::ring))
        {

            item.remove (At::ring);
            qDebug () << "Call : Incoming Ring";
            emit ringUpdate ();

        }

        if (item.contains (At::callEnd))
        {

            item.remove (At::callEnd);
            qDebug () << "Call : End";
            emit callEndUpdate ();

            this->outgoingCall = false;
            this->incomingCall = false;
        }

        if (item.contains (At::callReject))
        {

            item.remove (At::callReject);
            qDebug () << "Call : End";
            emit callEndUpdate ();

            this->outgoingCall = false;
            this->incomingCall = false;
        }

        if (item.contains (At::callAnswer))
        {
            item.remove (At::callAnswer);
            qDebug () << "Call : Outgoing Answer";
            emit callAnswerUpdate ();
        }

        if (item.contains (At::callAccept))
        {
            item.remove (At::callAccept);
            qDebug () << "Call : Incoming Answer";
            emit callAcceptUpdate ();
        }

        item = item.trimmed ();

        if (item.contains (At::systemInformationResponse))
        {

            item = item.remove (At::systemInformation).trimmed ();

            QStringList responseList = item.split (NEW_LINE);

            foreach (QString response, responseList)
            {

                if (response.contains ("Manufacturer:"))
                {

                    qDebug () << "Manufacturer:" <<response.split (COLON).at (1).trimmed ();
                    emit manufacturerUpdate (response.split (COLON).at (1).trimmed ());

                }
                else if (response.contains ("Model:"))
                {

                    qDebug () << "Device name:" <<response.split (COLON).at (1).trimmed ();
                    emit deviceNameUpdate (response.split (COLON).at (1).trimmed ());

                }
                else if (response.contains ("Revision:"))
                {

                    qDebug () << "Firmware Version:" <<response.split (COLON).at (1).trimmed ();
                    emit firmwareVersionUpdate (response.split (COLON).at (1).trimmed ());

                }
                else if (response.contains ("IMEI:"))
                {

                    qDebug () << "IMEI:" <<response.split (COLON).at (1).trimmed ();
                    emit imeiUpdate (response.split (COLON).at (1).trimmed ());

                }
            }

        }
        else if (item.contains (At::messageCenterNumberResponse))
        {

            item = item.remove (At::messageCenterNumber)
                   .remove (At::messageCenterNumberResponse)
                   .remove (QUOTES)
                   .trimmed ();

            qDebug () << "Message Centre Number:" << item.split (COMMA)[0].trimmed ();
            emit messageCenterNumberUpdate(item.split (COMMA)[0].trimmed ());

        }
        else if (item.contains (At::pinCodeStatusResponse))
        {

            int line = item.indexOf (NEW_LINE);
            item.remove (0, line+1);

            item = item.remove (At::pinCodeStatus)
                   .remove (At::pinCodeStatusResponse)
                   .trimmed ();

            qDebug () << "Pin code status:" << item;
            emit pinCodeStatusUpdate(item);

        }
        else if (item.contains (At::networkModeResponse))
        {

            item = item.remove (At::networkMode)
                   .remove (At::networkModeResponse)
                   .remove (QUOTES)
                   .trimmed ();

            QStringList list = item.split (NETWORK_MODE_SEPARATOR);

            foreach (QString response, list)
            {
                response = response.trimmed ();

                QStringList items = response.split (COMMA);
                if (5 > items.count ())
                {

                    continue;
                }

                qDebug () << "Network Mode Item" << items;
                emit networkModeUpdate (items[0].trimmed (),
                                        items[1].trimmed (),
                                        items[2].trimmed (),
                                        items[3].trimmed (),
                                        items[4].remove(CLOSE_PARANTHESIS)
                                        .trimmed ());

                if (response.contains (NETWORK_MODE_INVALID)) break;

            }

        }
        else if (item.contains (At::accessPointNameResponse))
        {

            item = item.remove (At::accessPointName)
                   .remove (At::accessPointNameResponse)
                   .remove (QUOTES).trimmed ();

            QStringList data = item.split (COMMA);
            if (3 > data.count ())
            {

                DeamonInterface::getInstance ()->write (
                                                       At::accessPointName);
                return (false);
            }

            qDebug () << "Access point name:" << data.at (2).trimmed ();
            emit accessPointNameUpdate(data.at (2).trimmed ());

        }
        else if (item.contains (At::contactCountResponse))
        {

            item = item.remove (At::contactCount)
                   .remove (At::contactCountResponse)
                   .remove (QUOTES).trimmed ();

            QStringList dataList = item.split (COMMA);
            if (3 > dataList.count ())
            {
                DeamonInterface::getInstance ()->write (
                                                       At::contactCount);
                return (false);
            }

            qDebug () << "Contact sim:"
            << dataList[1].trimmed ()
            << dataList[2].trimmed ();

            emit contactCountUpdate(dataList.at (1).trimmed (),
                                    dataList.at (2).trimmed ());

        }
        else if (item.contains (At::readContactResponse))
        {

            item = item.remove (At::readAllContact)
                   .remove (At::readContact)
                   .remove (At::readContactResponse)
                   .remove (QUOTES).trimmed ();

            QStringList dataList = item.split (NEW_LINE);

            foreach (QString data, dataList)
            {

                QStringList items = data.split (COMMA);
                if ( 4  > items.count ())
                {
                    continue;
                }

                this->readContactUpdate (
                                        Contact(items[3].trimmed (),
                                                items[1].trimmed (),
                                                "", "", "", "",
                                                Contact::Sim,
                                                items[0].toInt ()));
            }

        }
        else if (item.contains (At::messageCountResponse))
        {

            item = item.remove (At::messageCount)
                   .remove (At::messageCountResponse)
                   .remove (QUOTES).trimmed ();

            QStringList dataList = item.split (COMMA);
            if (3 > dataList.count ())
            {
                DeamonInterface::getInstance ()->write (
                                                       At::messageCount);
                return (false);
            }

            qDebug () << "message Sim:"
            << dataList.at (1).trimmed ()
            << dataList.at (2).trimmed ();

            emit messageCountUpdate (dataList.at (1).trimmed (),
                                     dataList.at (2).trimmed ());

        }
        else if (item.contains (At::listMessageResponse))
        {

            item = item.remove (At::listMessage).remove (QUOTES).trimmed ();

            item.remove (0, item.indexOf (At::listMessageResponse));

            QStringList list = item.split (At::listMessageResponse, QString::SkipEmptyParts);

            for (int index = 0; index < list.count (); index++)
            {

                QString data = list.at (index);
                DeamonInterface::getInstance ()->removeJunk(data);

                int separator = data.indexOf ('\n');
                if (0 > separator)
                {
                    separator = data.length ();
                }

                QStringList header = data.left (separator).split (COMMA, QString::SkipEmptyParts);

                QString content = data.right (data.size () - separator);
                DeamonInterface::removeJunk (content);

                QString number = header.at (2);
                QString name = DeamonInterface::getInstance ()->getName (number);
                QDateTime messageTime = QDateTime::currentDateTime ();

                if (4 < header.count ())
                {

                    QString string = header.at (3) + SPACE + header.at (4);
                    string.truncate (17);
                    messageTime = QDateTime::fromString (string, INPUT_DATETIME_FORMAT);

                }

                Message message (number, name, content.trimmed (),
                                 messageTime,
                                 Message::SimInbox,
                                 header.at (0).toInt (),
                                 false);

                if (header.at (1).contains (INBOX_READ))
                {

                }
                else if (header.at (1).contains (INBOX_UNREAD))
                {
                    message.setUnread (true);
                }
                else if (header.at (1).contains (OUTBOX_SENT))
                {
                    message.setType (Message::SimOutbox);
                }
                else if (header.at (1).contains (OUTBOX_UNSENT))
                {
                    message.setUnread (true);
                    message.setType (Message::SimOutbox);
                }

                emit listMessageUpdate (message);

            }

        }
        else if (item.contains (At::readMessageResponse))
        {

            item = item.remove (At::readMessageResponse)
                   .remove (At::readMessage)
                   .remove (QUOTES).trimmed ();

            QStringList responseList = item.split (NEW_LINE,  QString::SkipEmptyParts);
            int index = responseList[0].toInt();
            responseList.removeFirst ();

            QStringList header = responseList[0].split(COMMA, QString::SkipEmptyParts);
            responseList.removeFirst ();

            QString number = header.at (1);
            QString name = DeamonInterface::getInstance ()->getName (number);
            QDateTime messageTime = QDateTime::currentDateTime ();
            QString content = responseList.join (QString(NEW_LINE)).trimmed ();
            DeamonInterface::removeJunk (content);

            if (4 < header.count ())
            {

                QString string = header.at (3) + SPACE + header.at (4);
                string.chop (3);
                messageTime.fromString (string, INPUT_DATETIME_FORMAT);

            }

            Message message (number, name,
                             content,
                             messageTime,
                             Message::SimInbox, index, false);

            if (header.at (0).contains (INBOX_READ))
            {

            }
            else if (header.at (0).contains (INBOX_UNREAD))
            {
                message.setUnread (true);
            }
            else if (header.at (0).contains (OUTBOX_SENT))
            {
                message.setType (Message::SimOutbox);
            }
            else if (header.at (0).contains (OUTBOX_UNSENT))
            {
                message.setUnread (true);
                message.setType (Message::SimOutbox);
            }

            emit readMessageUpdate (message);

        }
        else if (item.contains (At::writeMessageResponse))
        {

            item = item.remove (At::writeMessageResponse).trimmed ();
            qDebug () << "Write Message Response" << item;

            emit writeMessageUpdate(item);


        }
        else if (item.contains (At::messageReceivedSim))
        {

            item = item.remove (At::messageReceivedSim).trimmed ();

            QStringList data = item.split (COMMA);
            if ( 2 > data.count ())
            {
                return (false);
            }

            qDebug () << "Sim Message Received" << data.at (1).trimmed ().toInt ();

            emit messageReceivedSimUpdate (data.at (1).trimmed ());

        }
        else if (item.contains (At::messageReceived))
        {

            item = item.remove (At::messageRecieved)
                   .remove (QUOTES).trimmed ();

            QStringList responseList = item.split (NEW_LINE,  QString::SkipEmptyParts);

            QStringList header = responseList[0].split(COMMA, QString::SkipEmptyParts);
            if (3 > header.count ()) return (false);

            responseList.removeFirst ();

            QString number = header[0].remove (QUOTES).trimmed ();
            QString content = responseList.join (QString(NEW_LINE)).trimmed ();
            DeamonInterface::getInstance ()->removeJunk (content);

            Message message (number,
                             DeamonInterface::getInstance ()->getName (number),
                             content,
                             QDateTime::fromString (header.at (1)
                                                    + SPACE + header[2]
                                                    .remove ("+22"),
                                                    INPUT_DATETIME_FORMAT),
                             Message::Inbox);

            emit messageRecievedUpdate(message);


        }
        else if (item.contains (At::deliveryReportSimResponse))
        {

            item = item.remove(At::deliveryReportSimResponse).trimmed ();

            emit deliveryReportSimUpdate (item);

        }
        else if (item.contains (At::deliveryReportResponse))
        {

            item = item.remove(At::deliveryReportResponse).
                   remove (QUOTES).trimmed ();

            QStringList data = item.split (COMMA);
            if ( 8 > data.count ())
            {
                return (false);
            }

            QString number = data.at (2).trimmed ();
            QDateTime time = QDateTime::fromString (data.at (6).trimmed () +
                                                    SPACE +
                                                    data[7].remove("+22").trimmed (),
                                                    INPUT_DATETIME_FORMAT);

            Message message (number,
                             DeamonInterface::getName (number),
                             MESSAGE_DELIVERY_TEXT,
                             time, Message::Report, true);

            emit deliveryReportUpdate(message);

        }
        else if (item.contains (At::supplementaryServicesResponse))
        {

            item = item.remove (At::supplementaryServices)
                   .remove (At::supplementaryServicesResponse)
                   .remove (QUOTES).trimmed ();

            QStringList data = item.split (COMMA);
            if (2 > data.count ())
            {
                return (false);
            }

            qDebug () << "check balance:" << data.at (1).trimmed ();
            emit checkBalanceUpdate(data.at (1).trimmed ());

        }
        else if (item.contains (At::hardwareSoftwareVersionResponse))
        {

            item = item.remove (At::hardwareSoftwareVersionResponse)
                   .remove (At::hardwareVersion).trimmed ();

            QStringList data = item.split (COMMA);

            foreach (QString response, data)
            {
                response = response.trimmed ();

                if (response.contains ("HW version:"))
                {

                    qDebug () << "HW version:" <<response.split (COLON).at (1).trimmed ();
                    emit hardwareVersionUpdate (response.split (COLON).at (1).trimmed ());

                }
                else if (response.contains ("SW version:"))
                {

                    qDebug () << "SW version:" <<response.split (COLON).at (1).trimmed ();
                    emit softwareVersionUpdate (response.split (COLON).at (1).trimmed ());

                }
            }

        }
        else if (item.contains (At::ownNumberResponse))
        {

            item = item.remove (At::ownNumber)
                   .remove (At::ownNumberResponse)
                   .remove (QUOTES).trimmed ();

            qDebug () << "MSISDN:" << item.split (COMMA)[1].trimmed ();
            emit ownNumberUpdate(item.split (COMMA)[1].trimmed ());


        }
        else if (item.contains (At::psAttachemntResponse))
        {

            item = item.remove (At::psAttachemnt).
                   remove (At::psAttachemntResponse).trimmed ();

            qDebug () << "PS Network Attachment" << item;
            emit psNetworkAttachmentUpdate (item);

        }
        else if (item.contains (At::psRegistrationResponse))
        {

            item = item.remove (At::psRegistration).
                   remove (At::psRegistrationResponse).trimmed ();


            qDebug () << "PS Regsitration" << item.split (COMMA).at (1);
            emit psNetworkRegistrationUpdate (item.split (COMMA).at (1));

        }
        else if (item.contains (At::pinPukAttemptsResponse))
        {

            item = item.remove (At::pinPukAttempts)
                   .remove (At::pinPukAttemptsResponse).trimmed ();

            emit pinAttemptUpdate(item.split (COMMA)[0].trimmed ());
            emit pukAttemptUpdate(item.split (COMMA)[1].trimmed ());

        }
        else if (item.contains (At::incomingCallResponse))
        {

            item = item.remove(At::incomingCallResponse).
                   remove ("RING").remove (QUOTES).trimmed ();

            qDebug () << "Incoming Call" << item.split (COMMA)[0].trimmed ();

            if (false == this->incomingCall)
            {

                emit incomingCallUpdate(Call(item.split (COMMA)[0].trimmed (),
                                             "", QDateTime::currentDateTime (),
                                             QTime(), Call::Missed));
                this->incomingCall = true;
            }

        }
        else if (item.contains (At::serialNumberResponse))
        {

            item = item.remove (At::serialNumber)
                   .remove (At::serialNumberResponse)
                   .remove (QUOTES).trimmed ();

            qDebug () << "Serial Number Update" << item;
            emit serialNumberUpdate(item);

        }
        else if (item.contains (At::pinQueryResponse))
        {

            item = item.remove(At::pinQueryResponse).
                   remove (At::pinQuery).trimmed ();

            emit pinQueryUpdate(item);

        }
        else if (item.contains (At::incorrectPinPukResponse))
        {

            int line = item.indexOf (NEW_LINE);
            item.remove (0, line+1);

            item = item.remove(At::incorrectPinPukResponse)
                   .remove (At::pinCodeStatus)
                   .remove (COLON).trimmed ();

            qDebug () << "incorrectPinPukResponse" << item;

            emit incorrectPinPukUpdate(item);

        }
        else if (item.contains (At::pinModifyResponse))
        {

            int line = item.indexOf (NEW_LINE);
            item.remove (0, line+1);

            item = item.remove (At::pinModify).
                   remove(At::pinModifyResponse).trimmed ();

            emit pinModifyUpdate(item);

        }
        else if (item.contains (At::getSelectedNetworkModeResponse))
        {

            item = item.remove(At::getSelectedNetworkMode)
                   .remove(At::getSelectedNetworkModeResponse).trimmed ();

            emit getSelectedNetworkModeUpdate(item);



        }
        else if (item.contains (At::writeContactResponse))
        {

            int line = item.indexOf (NEW_LINE);
            item.remove (0, line+1);

            item = item.remove (At::writeContact).
                   remove(At::writeContactResponse).trimmed ();

            qDebug () << "Write Contact Response" << item;

            emit writeContactUpdate(item);


        }
        else if (item.contains (At::cellIdentifierResponse))
        {

            item = item.remove (At::cellIdentifier)
                   .remove (At::cellIdentifierResponse).trimmed ();

            QStringList list = item.split (COMMA);
            if ( 3 > list.count () ) return (false);

            int location = (3 == list.count ())?0:1;

            qDebug () << "Cell identifier:" << list[location+2];
            emit cellIdentifierUpdate(list[location+2].trimmed ());

            qDebug () << "Home network:" << list[location];
            emit homeNetworkUpdate(list[location]);

            qDebug () << "CS Network Registration:" << list[location];
            emit csNetworkRegistrationUpdate(list[location]);

        }
        else if (item.contains (At::signalStrengthResponse))
        {

            item = item.remove (At::signalStrength).
                   remove (At::signalStrengthResponse).trimmed ();

            qDebug () << "Signal Strength" << item;
            emit signalStrengthUpdate (item);

        }
        else if (item.contains (At::networkNameResponse))
        {

            item = item.remove (At::networkName)
                   .remove (At::networkNameResponse)
                   .remove (QUOTES).trimmed ();

            QStringList list = item.split (COMMA);
            if ( 4 > list.count () )
            {

                return (false);
            }

            qDebug () << "Network name:" << list.at (2).trimmed ();
            emit networkNameUpdate(list[2].remove (QUOTES));
            emit networkSelectionModeUpdate(list.at (2).trimmed ());

            qDebug () << "Network Display Mode:" << list.at (3).trimmed () << list.at (2).trimmed ();
            emit networkDisplayModeUpdate(list.at (3).trimmed (), list.at (2).trimmed ());
        }

    }

    return (result);
}

bool DeamonInterface::stop ()
{
    this->running = false;

    return (true);
}

QString
DeamonInterface::getName (
                         QString number)
{
    QString name;

    if (number.startsWith ('0') || number.startsWith ('+'))
    {
        return (name);
    }

    if (number.count () > 10)
    {
        long long ascii = number.toLongLong ();

        while (ascii > 0)
        {
            int temp = ascii % 1000;

            if ( (temp > 127) || (temp < 100) )
            {

                temp = ascii % 100;
                ascii /= 100;

            }
            else
            {
                ascii /= 1000;
            }

            name.insert (0, temp);
        }
    }

    return (name);
}

bool
DeamonInterface::removeJunk (
                            QString& data)
{

    data.remove (At::signalStrength);
    data.remove (At::cellIdentifier);
    data.remove (At::networkName);

    int start = 0;
    int end = 0;

    start = data.indexOf (At::signalStrengthResponse);
    if (-1 != start)
    {
        end = data.indexOf ('\n', start);

        if ( -1 != end)
        {

            data.remove (start, end - start);

        }
        else
        {

            data.remove (start, data.length () - 1);
        }
    }

    start = data.indexOf (At::cellIdentifierResponse);
    if (-1 != start)
    {
        end = data.indexOf ('\n', start);

        if ( -1 != end)
        {

            data.remove (start, end - start);

        }
        else
        {

            data.remove (start, data.length () - 1);
        }
    }

    start = data.indexOf (At::networkNameResponse);
    if (-1 != start)
    {
        end = data.indexOf ('\n', start);

        if ( -1 != end)
        {

            data.remove (start, end - start);

        }
        else
        {

            data.remove (start, data.length () - 1);
        }
    }

    data = data.trimmed ();
    return (true);
}
