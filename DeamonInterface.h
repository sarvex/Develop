#ifndef DEAMON_INTERFACE
#define DEAMON_INTERFACE

#include "Call.h"
#include "Contact.h"
#include "Message.h"
#include "SerialPort.h"

#include <QByteArray>
#include <QQueue>
#include <QMutex>
#include <QString>
#include <QThread>
#include <QTimer>

class DeamonInterface
: public QThread
{
    Q_OBJECT

public:
    static DeamonInterface* getInstance ();

    bool write (QString value);
    bool enqueue (QString value);

    bool configure ();
    bool stop ();

    signals:
    bool signalStrengthUpdate (QString value);
    bool pinAttemptUpdate (QString value);
    bool pukAttemptUpdate (QString value);
    bool networkDisplayModeUpdate (QString value, QString networkName);

    bool incomingCallUpdate (Call call);

    bool messageRecievedUpdate (Message message);
    bool messageRecievedSimUpdate (QString index);
    bool deliveryReportUpdate (Message message);
    bool deliveryReportSimUpdate (QString index);

    bool readMessageUpdate (Message message);
    bool listMessageUpdate (Message message);
    bool readContactUpdate (Contact contact);

    bool ringUpdate ();
    bool stopRingUpdate ();
    bool ringBackUpdate ();

    bool callEndUpdate ();
    bool callAnswerUpdate ();
    bool callAcceptUpdate ();

    bool writeContactUpdate (QString index);
    bool writeMessageUpdate (QString index);

    bool manufacturerUpdate (QString value);
    bool firmwareVersionUpdate (QString value);
    bool imeiUpdate (QString value);
    bool getSelectedNetworkModeUpdate (QString value);

    bool deviceNameUpdate (QString value);
    bool interfacePortUpdate (QString value);
    bool serialNumberUpdate (QString value);
    bool softwareVersionUpdate (QString value);
    bool hardwareVersionUpdate (QString value);

    bool checkBalanceUpdate (QString value);
    bool incorrectPinPukUpdate (QString value);

    bool ownNumberUpdate (QString value);

    bool messageCountUpdate (QString count, QString total);
    bool contactCountUpdate (QString count, QString total);

    bool messageCenterNumberUpdate (QString value);
    bool pinCodeUpdate (QString value);
    bool pinQueryUpdate (QString value);

    bool networkNameUpdate (QString value);
    bool rssiUpdate (QString value);
    bool cellIdentifierUpdate (QString value);
    bool homeNetworkUpdate (QString value);
    bool csNetworkRegistrationUpdate (QString value);
    bool psNetworkRegistrationUpdate (QString value);
    bool psNetworkAttachmentUpdate (QString value);

    bool apnUpdate (QString value);
    bool networkSelectionModeUpdate (QString value);

    bool pinCodeStatusUpdate (QString value);
    bool pinModifyUpdate (QString value);

    bool accessPointNameUpdate (QString value);

    bool modemPlugged (bool status);

    bool newMessageUpdate (bool status);

    bool prlUpdate (QString value);
    bool esnUpdate (QString value);


    bool networkModeUpdate (QString mode,
                            QString major,
                            QString minor,
                            QString identifier,
                            QString status);

public slots:
    bool onSystemUpdateTimer ();

protected:
    virtual void run ();

private:
    explicit DeamonInterface(QObject *parent = 0);
    static DeamonInterface* singleton;

    QString getName (QString number);
    bool removeJunk (QString& data);

    bool checkPort ();
    bool openPort ();

    bool write ();
    bool read ();
    bool parse (const QByteArray& value);


    QString portName;
    SerialPort * port;

    QMutex * mutex;
    QQueue <QString> queue;
    QQueue <QString> waited;

    QTimer * systemInformationTimer;

    volatile bool running;

    bool plugged;

    bool incomingCall;
    bool outgoingCall;

    static const QString end;
};

#endif // DEAMON_INTERFACE
