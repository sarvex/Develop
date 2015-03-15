#ifndef DEVICE_WIDGET
#define DEVICE_WIDGET

#include <QFormLayout>
#include <QLineEdit>
#include <QString>
#include <QWidget>

class DeviceWidget : public QWidget {
	Q_OBJECT
public:
	explicit DeviceWidget(QWidget *parent = 0);

	QString readAll ();
	bool clear ();

signals:

public slots:
	bool onDeviceNameUpdate (QString value);
	bool onApplicationPortUpdate (QString value);
	bool onSerialNumberUpdate (QString value);
	bool onImeiUpdate (QString value);
	bool onHardwareVersionUpdate (QString value);
	bool onFirmwareVersionUpdate (QString value);

	bool onModemPlugged (bool status);
	bool onModemConnected (bool status);

protected:
	virtual void showEvent (QShowEvent* event);

private:
	QLineEdit * deviceName;
	QLineEdit * applicationPort;
	QLineEdit * serialNumber;
	QLineEdit * imei;
	QLineEdit * hardwareVersion;
	QLineEdit * firmwareVersion;
	QLineEdit * softwareVersion;

	QFormLayout * layout;
};

#endif // DEVICE_WIDGET
