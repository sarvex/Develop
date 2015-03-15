#include "At.h"
#include "DeamonInterface.h"
#include "DeamonSetting.h"
#include "DeamonDatabase.h"
#include "Define.h"

DeamonSetting*
DeamonSetting::singleton = NULL;

const Setting
DeamonSetting::defaultDeliveryReport = Setting ("", false);

const Setting
DeamonSetting::defaultMessageStorage = Setting ("", true);

DeamonSetting*
DeamonSetting::getInstance ()
{

    if (NULL == singleton)
    {

        singleton = new DeamonSetting();
    }

    return(singleton);
}

DeamonSetting::DeamonSetting (
                             QObject * parent)
: QObject(parent)
{

    QList <Setting> setting = DeamonDatabase::getInstance()->readSetting ();

    if ( setting.isEmpty ())
    {

        this->deliveryReport = DeamonSetting::defaultDeliveryReport;
        this->messageStorage = DeamonSetting::defaultMessageStorage;

        DeamonDatabase::getInstance ()->addSetting (
                                                   DeamonSetting::defaultDeliveryReport);

        DeamonDatabase::getInstance ()->addSetting (
                                                   DeamonSetting::defaultMessageStorage);

    }
    else
    {

        this->deliveryReport = setting.at (0);
        this->messageStorage = setting.at (1);

    }

    this->fireDeliveryReport ();
    this->fireMessageStorage ();
}

bool
DeamonSetting::setMessageStorage (
                                 bool messageStorage)
{

    this->messageStorage.setEnable (messageStorage);
    this->fireMessageStorage ();

    DeamonDatabase::getInstance ()->editSetting (this->messageStorage);

    return (true);
}

bool
DeamonSetting::fireMessageStorage ()
{

    if ( true == messageStorage.getEnable ())
    {

        DeamonInterface::getInstance ()->write (
                                               At::setMessageApplication);

    }
    else
    {

        DeamonInterface::getInstance ()->write (
                                               At::setMessageSim);
    }

    return (true);
}

bool
DeamonSetting::setDeliveryReport(
                                bool deliveryReport)
{

    this->deliveryReport.setEnable (deliveryReport);
    this->fireDeliveryReport ();

    DeamonDatabase::getInstance ()->editSetting (this->deliveryReport);

    return (true);
}

bool
DeamonSetting::fireDeliveryReport ()
{

    if ( true == deliveryReport.getEnable ())
    {

        DeamonInterface::getInstance ()->write (
                                               At::enableDeliveryAcknowledgement);

    }
    else
    {

        DeamonInterface::getInstance ()->write (
                                               At::disableDeliveryAcknowledgement);
    }

    return (true);
}

bool
DeamonSetting::getMessageStorage ()
{

    return (this->messageStorage.getEnable());
}

bool
DeamonSetting::getDeliveryReport() const
{

    return (this->deliveryReport.getEnable ());
}
