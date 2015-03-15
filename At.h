#ifndef AT
#define AT

#include <QString>

class At
{

public:
    static const QString at;
    static const QString end;
    static const QString dial;
    static const QString answer;
    static const QString disconnectCall;

    static const QString dtmf;
    static const QString duration;

    static const QString ok;
    static const QString error;
    static const QString dormant0;
    static const QString dormant1;

    static const QString setEchoOn;

    static const QString setCellIdentifier;
    static const QString cellIdentifier;
    static const QString cellIdentifierResponse;

    static const QString setDeviceMode;
    static const QString setDeviceModeResponse;

    static const QString setFullModemFunctionality;
    static const QString setFullModemFunctionalityResponse;

    static const QString setNetworkModeGsm;
    static const QString setNetworkModeGsmResponse;

    static const QString setMessageApplication;
    static const QString setMessageSim;
    static const QString setMessageMemorySim;

    static const QString setContactSim;

    static const QString setAccessPointName;

    static const QString setMessageModeText;
    static const QString setMessageModeTextResponse;


    static const QString signalStrength;
    static const QString signalStrengthResponse;

    static const QString meid;
    static const QString meidResponse;

    static const QString esn;
    static const QString esnResponse;

    static const QString systemInformation;
    static const QString systemInformationResponse;

    static const QString supplementaryServices;
    static const QString supplementaryServicesResponse;

    static const QString imei;
    static const QString imeiResponse;

    static const QString serialNumber;
    static const QString serialNumberResponse;

    static const QString hardwareVersion;
    static const QString hardwareSoftwareVersionResponse;

    static const QString messageReceived;
    static const QString messageReceivedSim;

    static const QString messageCenterNumber;
    static const QString messageCenterNumberResponse;

    static const QString pinCodeStatus;
    static const QString pinCodeStatusResponse;

    static const QString networkName;
    static const QString networkNameResponse;

    static const QString accessPointName;
    static const QString accessPointNameResponse;

    static const QString messageCount;
    static const QString messageCountResponse;

    static const QString setValidity;

    static const QString readMessage;
    static const QString readMessageResponse;

    static const QString listMessage;
    static const QString listMessageResponse;

    static const QString newMessage;
    static const QString newMessageResponse;

    static const QString writeMessage;
    static const QString writeMessageResponse;

    static const QString deleteMessage;
    static const QString deleteMessageResponse;

    static const QString editMessage;
    static const QString editMessageResponse;

    static const QString contactCount;
    static const QString contactCountResponse;

    static const QString writeContact;
    static const QString writeContactResponse;

    static const QString readContact;
    static const QString readAllContact;
    static const QString readContactResponse;

    static const QString deleteContact;

    static const QString editContact;
    static const QString editContactResponse;

    static const QString ownNumber;
    static const QString ownNumberResponse;

    static const QString networkMode;
    static const QString networkModeResponse;

    static const QString psAttachemnt;
    static const QString psAttachemntResponse;

    static const QString psRegistration;
    static const QString psRegistrationResponse;

    static const QString pinPukAttempts;
    static const QString pinPukAttemptsResponse;

    static const QString incomingCallResponse;

    static const QString ring;
    static const QString stopRing;
    static const QString ringBack;

    static const QString callEnd;
    static const QString callReject;
    static const QString callAnswer;
    static const QString callAccept;

    static const QString networkDisplayMode;
    static const QString networkDisplayModeResponse;

    static const QString pinServices;
    static const QString enterPin;

    static const QString pinQuery;
    static const QString pinQueryResponse;

    static const QString pinModify;
    static const QString pinModifyResponse;

    static const QString enableDeliveryAcknowledgement;
    static const QString disableDeliveryAcknowledgement;

    static const QString deliveryReportResponse;
    static const QString deliveryReportSimResponse;

    static const QString automaticNetworkSelection;
    static const QString manualNetworkSelection;

    static const QString incorrectPinPukResponse;
    static const QString changeNetworkMode;

    static const QString getSelectedNetworkMode;
    static const QString getSelectedNetworkModeResponse;
};

#endif // AT
