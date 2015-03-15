#ifndef HOME_WIDGET
#define HOME_WIDGET

#include "AnimateButton.h"
#include "ImageButton.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QPixmap>
#include <QVBoxLayout>

class HomeWidget
	: public QWidget {
	Q_OBJECT

public:
	explicit HomeWidget (QWidget *parent = 0);

signals:
	bool dial  (QString number,
				bool connect);

public slots:
    bool onResetNetwork();
	bool onConnectButtonClicked ();
	bool onDisconnectButtonClicked ();
	bool onCustomerSupportCallButtonClicked ();
	bool onCustomerSupportLinkButtonClicked ();
	bool onPrepaidLinkButtonClicked ();
	bool onPostpaidLinkButtonClicked ();
	bool onSignalStrengthUpdate (int value);
	bool onHomeNetworkUpdate (QString value);
	bool onPinCodeUpdate (QString value);

	bool onModemPlugged (bool status);
	bool onModemConnected (bool status);

protected:
	virtual void paintEvent (QPaintEvent* event);

private:
	QImage * background;

	QLabel * signalStrengthBackground;
	QLabel * bannerLabel;
	QLabel * signalStrengthLabel;
	QLabel * homeNetworkLabel;
	QLabel * roamingNetworkLabel;
	QLabel * customerSupportBackground;
	QLabel * bill;

	AnimateButton * connectingButton;
	AnimateButton * disconnectingButton;

	ImageButton * connectButton;
	ImageButton * disconnectButton;
	ImageButton * customerSupportCallButton;
	ImageButton * customerSupportLinkButton;
	ImageButton * customerCareButton;
	ImageButton * prepaidLinkButton;
	ImageButton * postpaidLinkButton;

	QVBoxLayout * buttonLayout;
	QHBoxLayout * layout;

	QList <QPixmap> signalStrengthImage;
	QList <QPixmap> homeNetworkList;
	QList <QPixmap> roamingNetworkList;
};

#endif // HOME_WIDGET
