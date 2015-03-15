#ifndef EJECT_H
#define EJECT_H

#include <QObject>

#include <windows.h>

class Eject : public QObject
{
    Q_OBJECT
public:
    explicit Eject(QObject *parent = 0);

    void ejectDrive(TCHAR driveletter);
    bool matchDevInstToUsbDevice(DWORD device, DWORD vid, DWORD pid);
    void usbEjectDevice(unsigned vid, unsigned pid);
    DWORD GetDrivesDevInstByDeviceNumber(long DeviceNumber);

signals:

public slots:

protected:

private:
};

#endif // EJECT_H
