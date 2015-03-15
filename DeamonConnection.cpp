#include "Data.h"
#include "DeamonConnection.h"
#include "DeamonDatabase.h"
#include "DeamonSetting.h"
#include "Define.h"

#include <QDateTime>
#include <QDebug>

#ifdef Q_WS_WIN

    #include <setupapi.h>
    #include <devguid.h>
    #include <initguid.h>

    #define errorMessageSize 256

static HRASCONN rasConnection = NULL;
static RASCONNSTATE rasConnectionState;
static DWORD longParameter = 0;

#endif // Q_WS_WIN

#ifndef GUID_DEVCLASS_PORTS
DEFINE_GUID (GUID_DEVCLASS_MODEM,
             0x4d36e96d,
             0xe325, 0x11ce,
             0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18);
#endif

DeamonConnection* DeamonConnection::singleton = NULL;

DeamonConnection*
DeamonConnection::getInstance ()
{

    if (NULL == singleton)
    {

        singleton = new DeamonConnection();
    }

    return(singleton);
}

DeamonConnection::DeamonConnection(
                                  QObject * parent) :
QObject(parent),

bytesTransmitted (0.0),
bytesRecieved (0.0),

transmitOffset (0.0),
receiveOffset (0.0),

duration (0.0),
durationOffset (0.0)
{

    this->timer = new QTimer;

    QObject::connect (this->timer,
                      SIGNAL(timeout()),
                      this,
                      SLOT(onTimer()));

    QObject::connect (this,
                      SIGNAL(modemConnected (bool)),
                      this,
                      SLOT (onModemConnected (bool)));
}

QString
DeamonConnection::getModemName ()
{

    QString modemRevision (MODEM_REVISION);
    QString modem (MODEM);

    SP_DEVINFO_DATA deviceInformationData;

    deviceInformationData.cbSize = sizeof(SP_DEVINFO_DATA);

    HDEVINFO deviceInformation = SetupDiGetClassDevs (
                                                     (LPGUID)&GUID_DEVCLASS_MODEM, 0, 0, DIGCF_PRESENT);

    if (INVALID_HANDLE_VALUE == deviceInformation)
    {

        return ("");
    }

    DWORD size = 1024;
    BYTE data[1024];

    for (int index = 0;
        SetupDiEnumDeviceInfo(deviceInformation, index, &deviceInformationData);
        index++)
    {

        if (false == SetupDiGetDeviceRegistryProperty (
                                                      deviceInformation, &deviceInformationData,
                                                      SPDRP_HARDWAREID, NULL, data, 1024, &size))
        {

            qDebug () << "SetupDiGetDeviceRegistryProperty" << GetLastError ();
            continue;
        }

        if ( (false == modemRevision.contains (
                                              QString::fromWCharArray (
                                                                      (const wchar_t*)data), Qt::CaseInsensitive))
             && (false == modem.contains (
                                         QString::fromWCharArray (
                                                                 (const wchar_t*)data), Qt::CaseInsensitive)) )
        {

            continue;
        }

        if ( false == SetupDiGetDeviceRegistryProperty (
                                                       deviceInformation, &deviceInformationData,
                                                       SPDRP_FRIENDLYNAME, NULL, data, 1024, &size))
        {

            continue;
        }

        qDebug () << "Modem Selected" << QString::fromWCharArray ((const wchar_t*)data);

        return (QString::fromWCharArray ((const wchar_t*)data));
    }

    return ("");
}

bool
DeamonConnection::connectDataCall ()
{

    bool result = false;

    this->bytesRecieved = 0.0;
    this->bytesTransmitted = 0.0;

    this->transmitOffset = 0.0;
    this->receiveOffset = 0.0;

    this->duration = 0.0;
    this->durationOffset = 0.0;

#ifdef Q_WS_WIN

    // create and initialize dial up parameters
    RASDIALPARAMS rasDialParameters;
    ZeroMemory(&rasDialParameters, sizeof(RASDIALPARAMS));
    rasDialParameters.dwSize = sizeof(RASDIALPARAMS);

    // set connection name
    QString name (DIALUP_CONNECTION_NAME);
    name.toWCharArray(rasDialParameters.szEntryName);

    // set connection username
    QString username (DIALUP_USERNAME);
    username.toWCharArray(rasDialParameters.szUserName);

    // set connection password
    QString password (DIALUP_PASSWORD);
    password.toWCharArray(rasDialParameters.szPassword);

    rasDialParameters.szCallbackNumber[0] = ' ';
    rasDialParameters.szDomain[0] = '*';

    rasConnection = NULL;
    longParameter = -1;

    // connect the call
    DWORD returnValue = RasDial (NULL,
                                 NULL,
                                 &rasDialParameters,
                                 0L,
                                 (LPVOID)RasDialFunc,
                                 &rasConnection);

    if (ERROR_CANNOT_FIND_PHONEBOOK_ENTRY == returnValue)
    {

        if (false == this->createConnectionEntry())
        {

            emit modemConnected (false);
            return (false);

        }
        else
        {

            returnValue = RasDial (NULL,
                                   NULL,
                                   &rasDialParameters,
                                   0L,
                                   (LPVOID)RasDialFunc,
                                   &rasConnection);

            qDebug () << "Ras Dial :" << returnValue;
        }
    }

    if (returnValue)
    {

        result = true;

    }
    else
    {

        WCHAR errorMessage[errorMessageSize];
        if (ERROR_SUCCESS  ==
            RasGetErrorString(returnValue, errorMessage, errorMessageSize))
        {
            qDebug () << "Error Message :" << "returnValue :"
            << returnValue << " : "
            << QString::fromWCharArray(errorMessage);

        }
        else
        {

            qDebug () << "Failure getting RAS error message = " << returnValue;
        }
    }
#endif // Q_WS_WIN

    return(result);
}

bool
DeamonConnection::disconnectDataCall()
{

    bool result = false;

#ifdef Q_WS_WIN

    // create and initialize ras connection status
    RASCONNSTATUS connectionStatistics;
    ZeroMemory(&connectionStatistics, sizeof(RASCONNSTATUS));
    connectionStatistics.dwSize = sizeof(RASCONNSTATUS);

    DWORD returnValue =
    RasGetConnectStatus(rasConnection, &connectionStatistics);

    if (ERROR_INVALID_HANDLE != returnValue)
    {

        returnValue = RasHangUp(rasConnection);
        qDebug () << "RasHangUp Return Value : " << returnValue;

        returnValue = RasGetConnectStatus(rasConnection,
                                          &connectionStatistics);

        if ( (ERROR_INVALID_HANDLE == returnValue)
             || (ERROR_PORT_DISCONNECTED == returnValue)
             || (ERROR_ALREADY_DISCONNECTING == returnValue)
             || (ERROR_DISCONNECTION == returnValue)
             || (ERROR_REMOTE_DISCONNECTION == returnValue)
             || (ERROR_HARDWARE_FAILURE == returnValue)
             || (ERROR_USER_DISCONNECTION == returnValue)
             || (ERROR_INVALID_PORT_HANDLE == returnValue)
             || (ERROR_PORT_NOT_CONNECTED == returnValue)
           )
        {
            rasConnection = NULL;
            emit modemConnected (false);
            result = false;
        }


    }

    Data data (this->bytesRecieved / (1024 * 1024),
               this->bytesTransmitted / (1024*1024),
               QDateTime::currentDateTime ());

    DeamonDatabase::getInstance ()->addData (data);

#endif // Q_WS_WIN

    return(result);
}

bool
DeamonConnection::createConnectionEntry()
{

#ifdef Q_WS_WIN

    DWORD dwError = ERROR_SUCCESS;

    // Allocate heap memory and initialize RASENTRY structure
    RASENTRY rasEntry;
    ZeroMemory(&rasEntry, sizeof(RASENTRY));

    // set dwSize to sizeof(RASENTRY) to identify the version of the structure
    rasEntry.dwSize = sizeof(RASENTRY);

    QString number (DIALUP_NUMBER);
    number.toWCharArray(rasEntry.szLocalPhoneNumber);

    QString modemName = this->getModemName ();
    if (modemName.isEmpty ()) modemName = OLIVE_MODEM_NAME;
    modemName.toWCharArray(rasEntry.szDeviceName);

    QString modemType(OLIVE_DEVICE_TYPE);
    modemType.toWCharArray(rasEntry.szDeviceType);

    // set the Point-to-Point Protocol (PPP). protocol
    rasEntry.dwFramingProtocol = RASFP_Ppp;

    // try to negotiate both IPv4 and IPv6 protocols
    rasEntry.dwfNetProtocols = RASNP_Ip | 0x00000008; //RASNP_Ipv6

    // set the default connection options
    rasEntry.dwfOptions = RASEO_RemoteDefaultGateway
                          | RASEO_SwCompression
                          | RASEO_RequirePAP
                          | RASEO_RequireCHAP
                          | RASEO_RemoteDefaultGateway
                          | RASEO_IpHeaderCompression;

    rasEntry.dwfOptions2 |= RASEO2_DontNegotiateMultilink;

    rasEntry.dwType = RASET_Phone;
    rasEntry.dwIdleDisconnectSeconds = RASIDS_Disabled;

    if (dwError != S_OK)
    {
        qDebug() << "Structure initilization failed: Error = " << dwError;
        return(false);
    }

    QString entryName (DIALUP_CONNECTION_NAME);
    LPCTSTR rasEntryName = (LPCTSTR) entryName.utf16 ();

    // Validate the RAS entry
    switch (RasValidateEntryName(NULL, rasEntryName))
    {
        
        case ERROR_SUCCESS: {

                qDebug() << "Entry name: "<< QString::fromWCharArray(rasEntryName)
                << "is valid but doesn't exist in the default phone book";
            }break;

        case ERROR_INVALID_NAME:{

                qDebug() << "Entry name: "<< QString::fromWCharArray(rasEntryName)
                << " is invalid\n";
                return(false);
            } break;

        case ERROR_ALREADY_EXISTS: {

                qDebug() << "Entry name: "<< QString::fromWCharArray(rasEntryName)
                << " already exists in the default phone book";
                return(false);
            }break;

        default:{

                qDebug() << "RasValidateEntryName failed: Unknown Error";
                return(false);
            }break;

    }

    qDebug() << "Create and set the new entry's properties";
    if (ERROR_SUCCESS != RasSetEntryProperties(NULL,
                                               rasEntryName,
                                               &rasEntry,
                                               rasEntry.dwSize,
                                               NULL,
                                               0))
    {
        qDebug () << "RasSetEntryProperties Failed";
    }

#endif // Q_WS_WIN

    return(true);
}

bool
DeamonConnection::readConnectionEntry ()
{

#ifdef Q_WS_WIN

    DWORD returnValue = ERROR_SUCCESS;

    QString entryName (DIALUP_CONNECTION_NAME);
    LPCTSTR rasEntryName = (LPCTSTR) entryName.utf16 ();

    // Allocate heap memory and initialize RASENTRY structure
    RASENTRY rasEntry;
    ZeroMemory(&rasEntry, sizeof(RASENTRY));

    rasEntry.dwSize = sizeof(RASENTRY);

    returnValue = RasGetEntryProperties(NULL,
                                        rasEntryName,
                                        &rasEntry,
                                        &rasEntry.dwSize,
                                        NULL,
                                        NULL);

    if (ERROR_SUCCESS != RasSetEntryProperties(NULL,
                                               rasEntryName,
                                               &rasEntry,
                                               rasEntry.dwSize,
                                               NULL,
                                               0))
    {

        qDebug () << "RasGetEntryProperties Failed";
        return (false);
    }

#endif // Q_WS_WIN

    return(true);
}

bool
DeamonConnection::removeConnectionEntry ()
{
#ifdef Q_WS_WIN

    QString entryName (DIALUP_CONNECTION_NAME);
    LPCTSTR rasEntryName = (LPCTSTR) entryName.utf16 ();

    // Clean up: delete the new entry
    DWORD result = RasDeleteEntry(NULL, rasEntryName);
    if (ERROR_SUCCESS != result)
    {
        qDebug () << "Unable to delete Ras Entry"
        << QString::fromWCharArray (rasEntryName) << result;
        return(false);
    }
#endif // Q_WS_WIN

    return (true);
}

bool
DeamonConnection::onResetUsage ()
{

    this->transmitOffset = this->bytesTransmitted;
    this->receiveOffset = this->bytesRecieved;
    this->durationOffset = this->duration;

    return (true);
}

bool DeamonConnection::onModemConnected (bool status)
{

    if (true == status)
    {

        this->timer->start (1000);

    }
    else
    {

        this->timer->stop ();
    }

    return (true);
}

bool DeamonConnection::onTimer ()
{

    RASCONNSTATUS connectionState;
    ZeroMemory(&connectionState, sizeof(RASCONNSTATUS));
    connectionState.dwSize = sizeof(RASCONNSTATUS);

    int result = RasGetConnectStatus(rasConnection, &connectionState);


    if ( (ERROR_INVALID_HANDLE == result) ||
         (RASCS_Disconnected == connectionState.rasconnstate))
    {

        this->timer->stop ();
        emit modemConnected (false);
        return (false);
    }

    RAS_STATS connectionStatistics;
    ZeroMemory(&connectionStatistics, sizeof(RAS_STATS));
    connectionStatistics.dwSize = sizeof(RAS_STATS);

    RasGetConnectionStatistics(rasConnection, &connectionStatistics);

    double bytesTransmitted = (connectionStatistics.dwBytesXmited/1024)
                              - this->bytesTransmitted
                              - this->transmitOffset;

    double bytesRecieved = (connectionStatistics.dwBytesRcved/1024)
                           - this->bytesRecieved
                           - this->recieveOffset;

    double duration = (connectionStatistics.dwConnectDuration/1000)
                      - this->duration
                      - this->durationOffset;

    if ( 0.0 == duration)
    {
        this->timer->stop ();
        emit modemConnected (false);
        return (false);
    }

    this->bytesTransmitted = (connectionStatistics.dwBytesXmited/1024)
                             - this->transmitOffset;

    this->bytesRecieved = (connectionStatistics.dwBytesRcved/1024)
                          - this->recieveOffset;

    this->duration = (connectionStatistics.dwConnectDuration / 1000)
                     - this->durationOffset;

    emit usageUpdate (bytesTransmitted, bytesRecieved);
    emit speedUpdate (bytesTransmitted/duration, bytesRecieved/duration);

    emit durationUpdate (QTime(0,0).addSecs (this->duration));

    return (true);
}

#ifdef Q_WS_WIN
void
CALLBACK RasDialFunc(
                    UINT index,
                    RASCONNSTATE
                    connectionState, DWORD error)
{
    Q_UNUSED(index);

    longParameter = error;
    rasConnectionState = connectionState;

    switch (longParameter)
    {
        case ERROR_SERVER_NOT_RESPONDING: {
                qDebug("Server Not Responding");
            }break;
        case ERROR_NETBIOS_ERROR: {
                qDebug("Netbios Error");
            }break;
        case ERROR_AUTH_INTERNAL: {
                qDebug("Internal Authentication Error");
            }break;
        case ERROR_CANNOT_GET_LANA: {
                qDebug("Cannot get LANA");
            }break;
    }

    if (longParameter)
    {
        WCHAR errorMessage[errorMessageSize];

        if (ERROR_SUCCESS == RasGetErrorString(longParameter,
                                               errorMessage,
                                               errorMessageSize))
        {

            emit DeamonConnection::getInstance ()->rasError(
                                                           longParameter, QString::fromWCharArray(errorMessage));
            qDebug () << longParameter << ":"
            << QString::fromWCharArray(errorMessage);
        }
        else
        {

            qDebug () << " RasDialFunc Failure getting RAS error"
            << longParameter;
        }

        emit DeamonConnection::getInstance ()->modemConnected (false);
        qDebug () << "emitted modemDisconnected";
    }

    switch (rasConnectionState)
    {
        case RASCS_OpenPort: {
                qDebug () << "Opening Port";
            }break;
        case RASCS_PortOpened: {
                qDebug () << "Port Opened";
            }break;
        case RASCS_ConnectDevice: {
                qDebug () << "Connecting Device";
            }break;
        case RASCS_DeviceConnected:{
                qDebug () << "Device Connected";
            }break;
        case RASCS_AllDevicesConnected: {
                qDebug () << "All Devices Connected";
            }break;
        case RASCS_Authenticate: {
                qDebug () << "Authenticating";
            }break;
        case RASCS_AuthNotify: {
                qDebug () << "Authentication Notify";
            }break;
        case RASCS_AuthRetry: {
                qDebug () << "Authentication Retry";
            }break;
        case RASCS_AuthCallback: {
                qDebug () << "Authentication Call Back";
            }break;
        case RASCS_AuthChangePassword:{
                qDebug () << "Authentication Change Password";
            }break;
        case RASCS_AuthProject:{
                qDebug () << "Authenticating Project";
            }break;
        case RASCS_AuthLinkSpeed:{
                qDebug () << "Authenticating Link Speed";
            }break;
        case RASCS_AuthAck:{
                qDebug () << "Authentication Acknowledge";
            }break;
        case RASCS_ReAuthenticate:{
                qDebug () << "Reauthentication";
            }break;
        case RASCS_Authenticated:{
                qDebug () << "Authenticated";
            }break;
        case RASCS_PrepareForCallback:{
                qDebug () << "Prepare For Callback";
            }break;
        case RASCS_WaitForModemReset:{
                qDebug () << "Waiting for Modem Reset";
            }break;
        case RASCS_WaitForCallback: {
                qDebug () << "Waiting for Callback";
            }break;
        case RASCS_Projected: {
                qDebug () << "Projected";
            }break;
        case RASCS_SubEntryConnected: {
                qDebug () << "Sub Entry Connected";
            }break;
        case RASCS_SubEntryDisconnected: {
                qDebug () << "Sub Entry Disconnected";
            }break;
        case RASCS_Interactive: {
                qDebug () << "Interactive";
            }break;
        case RASCS_RetryAuthentication: {
                qDebug () << "Retry Authentication";
            }break;
        case RASCS_CallbackSetByCaller:{
                qDebug () << "Callback Set by Caller";
            }break;
        case RASCS_PasswordExpired:{
                qDebug () << "Password Expired";
            }break;
        case RASCS_Connected: {
                qDebug () << "Connected";
                emit DeamonConnection::getInstance ()->modemConnected (true);
            }break;
        case RASCS_Disconnected: {
                qDebug () << "Disconnected";
                emit DeamonConnection::getInstance ()->modemConnected (false);
            }break;
        case RASCS_StartAuthentication: {
                qDebug () << "Start Authentication";
            }break;
        case RASCS_CallbackComplete: {
                qDebug () << "Callback Complete";
            }break;
        case RASCS_LogonNetwork: {
                qDebug () << "Logon Network";
            }break;
        case RASCS_InvokeEapUI:{
                qDebug () << "Invoke EapUI";
            }break;
    }
}

#endif // Q_WS_WIN
