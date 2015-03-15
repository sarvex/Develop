#ifndef NETWORK_STATUS_WIDGET
#define NETWORK_STATUS_WIDGET

#include <QFormLayout>
#include <QLineEdit>
#include <QString>
#include <QWidget>

class NetworkStatusWidget
		: public QWidget {
	Q_OBJECT

public:
	explicit NetworkStatusWidget(QWidget *parent = 0);

	QString readAll ();
	bool clear ();

signals:

public slots:
	bool onNetworkNameUpdate (QString value);
	bool onRssiUpdate (QString value);
	bool onCellIdentifierUpdate (QString value);
	bool onCsNetworkRegistrationUpdate (QString value);
	bool onPsNetworkRegistrationUpdate (QString value);
	bool onPsNetworkAttachmentUpdate (QString value);

	bool onModemPlugged (bool status);
	bool onModemConnected (bool status);

    bool onTimer();

protected:
	virtual void showEvent (QShowEvent* event);

private:
	QLineEdit * networkName;
	QLineEdit * rssi;
	QLineEdit * cellIdentifier;
	QLineEdit * csNetworkRegistration;
	QLineEdit * psNetworkRegistration;
	QLineEdit * psNetworkAttachment;

	QFormLayout * layout;

	QStringList networkAttachment;
	QStringList networkRegistration;
	QStringList networkStatus;
};

#endif // NETWORK_STATUS_WIDGET
