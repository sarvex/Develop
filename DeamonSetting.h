#ifndef DEAMON_SETTING
#define DEAMON_SETTING

#include "Setting.h"

#include <QObject>
#include <QString>

class DeamonSetting : public QObject
{
    Q_OBJECT
public:
    static DeamonSetting* getInstance ();

    bool getDeliveryReport () const;
    bool setDeliveryReport (bool deliveryReport);

    bool getMessageStorage ();
    bool setMessageStorage (bool messageStorage);

    signals:

public slots:

protected:

private:
    explicit DeamonSetting(QObject *parent = 0);
    static DeamonSetting* singleton;

    bool fireMessageStorage ();
    bool fireDeliveryReport ();

    Setting deliveryReport;
    Setting messageStorage;

    const static Setting defaultDeliveryReport;
    const static Setting defaultMessageStorage;
};

#endif // DEAMON_SETTING
