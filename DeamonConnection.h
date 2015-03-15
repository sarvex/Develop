#ifndef DEAMON_CONNECTION
#define DEAMON_CONNECTION

#include <QObject>
#include <QTime>
#include <QTimer>

#ifdef Q_WS_WIN
    #include <windows.h>
    #include <ras.h>
    #include <raserror.h>
#endif // Q_WS_WIN

class DeamonConnection
: public QObject
{
    Q_OBJECT

public:
    static DeamonConnection* getInstance ();

    bool connectDataCall ();
    bool disconnectDataCall ();

    bool createConnectionEntry ();
    bool removeConnectionEntry ();
    bool readConnectionEntry ();

    signals:
    bool modemConnected (bool status);

    bool rasError (int errorNumber, QString errorMessage);

    bool usageUpdate (double upload, double download);
    bool speedUpdate (double upload, double download);
    bool durationUpdate (QTime duration);

public slots:
    bool onResetUsage ();
    bool onTimer ();

    bool onModemConnected (bool status);

protected:

private:
    explicit DeamonConnection (QObject *parent = 0);
    static DeamonConnection* singleton;

    QString getModemName ();

    double bytesTransmitted;
    double bytesRecieved;

    double transmitOffset;
    double recieveOffset;

    double duration;
    double durationOffset;

    QTimer * timer;


#ifdef Q_WS_WIN
    friend void  CALLBACK RasDialFunc(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError);
#endif // Q_WS_WIN
};

#ifdef Q_WS_WIN
void CALLBACK RasDialFunc(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError);
#endif // Q_WS_WIN

#endif // DEAMON_CONNECTION
