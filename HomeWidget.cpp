#include "HomeWidget.h"

#include "DeamonConnection.h"
#include "DeamonInterface.h"
#include "Define.h"
#include "PinActivateDialog.h"
#include "PinUnlockDialog.h"
#include <QMessageBox>

#include <QDebug>
#include <QDesktopServices>
#include <QPainter>
#include <QUrl>

HomeWidget::HomeWidget (
		QWidget *parent)
        : QWidget(parent),
        modemPlugged (false) {

	this->background = new QImage (":/homeBackground.png");

	this->bannerLabel = new QLabel (this);
	this->bannerLabel->setPixmap (QPixmap(":/bannerDefault.png"));
	this->bannerLabel->setGeometry (25, 25, 487, 62);

	this->signalStrengthBackground = new QLabel (this);
	this->signalStrengthBackground->setPixmap (QPixmap(":/signalStrength.png"));
	this->signalStrengthBackground->setGeometry (545, 0, 204, 168);

	this->signalStrengthImage << QPixmap(":/signal0.png");
	this->signalStrengthImage << QPixmap(":/signal1.png");
	this->signalStrengthImage << QPixmap(":/signal2.png");
	this->signalStrengthImage << QPixmap(":/signal3.png");
	this->signalStrengthImage << QPixmap(":/signal4.png");
	this->signalStrengthImage << QPixmap(":/signal5.png");

	this->signalStrengthLabel = new QLabel (this);
	this->signalStrengthLabel->setPixmap (this->signalStrengthImage.at(0));
	this->signalStrengthLabel->setToolTip ("Signal Strength");
	this->signalStrengthLabel->setGeometry (650, 4, 90, 60);

	this->homeNetworkList << QPixmap (":/homeDisable.png");
	this->homeNetworkList << QPixmap (":/homeEnable.png");

	this->homeNetworkLabel = new QLabel (this);
	this->homeNetworkLabel->setPixmap (this->homeNetworkList.at (0));
	this->homeNetworkLabel->setToolTip ("Home Network");
	this->homeNetworkLabel->setGeometry (549, 85, 99, 77);

	this->roamingNetworkList << QPixmap (":/roamDisable.png");
	this->roamingNetworkList << QPixmap (":/roamEnable.png");

	this->roamingNetworkLabel = new QLabel (this);
	this->roamingNetworkLabel->setPixmap (this->roamingNetworkList.at (0));
	this->roamingNetworkLabel->setToolTip ("Roaming Network");
	this->roamingNetworkLabel->setGeometry (644, 85, 99, 77);

	this->connectButton = new ImageButton ("",
										   ":/connectDefault.png",
										   ":/connectHover.png",
										   ":/connectPress.png",
										   false, this);
	this->connectButton->setToolTip ("Connect to Internet");
	this->connectButton->setGeometry (545, 170, 204, 108);

	this->connectingButton = new AnimateButton(":/connecting1.png",
											   ":/connecting2.png",
											   ":/connecting3.png",
											   ":/connecting4.png",
											   ":/connecting5.png",
											   ":/connecting6.png",
											   this);
	this->connectingButton->setToolTip ("Connecting to Internet, Kindly Wait...");
	this->connectingButton->setGeometry (545, 170, 204, 108);
	this->connectingButton->hide ();

	this->disconnectButton = new ImageButton ("",
											  ":/disconnectDefault.png",
											  ":/disconnectHover.png",
											  ":/disconnectPress.png",
											  false, this);
	this->disconnectButton->setToolTip ("Disconnect from internet");
	this->disconnectButton->setGeometry (545, 170, 204, 108);
	this->disconnectButton->hide ();

	this->disconnectingButton = new AnimateButton(":/disconnecting1.png",
												  ":/disconnecting2.png",
												  ":/disconnecting3.png",
												  ":/disconnecting4.png",
												  ":/disconnecting5.png",
												  ":/disconnecting6.png",
												  this);
	this->disconnectingButton->setToolTip ("Disconnection from Internet. Kindly Wait...");
	this->disconnectingButton->setGeometry (545, 170, 204, 108);
	this->disconnectingButton->hide ();

	this->customerSupportBackground = new QLabel(this);
	this->customerSupportBackground->setPixmap (QPixmap(":/customerSupport.png"));
	this->customerSupportBackground->setToolTip ("Customer Care");
	this->customerSupportBackground->setGeometry (545, 280, 204, 119);

	this->customerCareButton = new ImageButton ("",
											":/custCareDefault.png",
											":/custCareHover.png",
											true, this);
	this->customerCareButton->setToolTip ("Call Customer Care");
	this->customerCareButton->setGeometry (553, 325, 19, 10);

	this->customerSupportLinkButton = new ImageButton ("",
													   ":/careLinkDefault.png",
													   ":/careLinkHover.png",
													   true, this);
	this->customerSupportLinkButton->setToolTip ("Visit Customer Care Website");
	this->customerSupportLinkButton->setGeometry (600, 323, 44, 14);

	this->customerSupportCallButton = new ImageButton ("",
													   ":/careDefault.png",
													   ":/careHover.png",
													   true, this);
	this->customerSupportCallButton->setToolTip ("Call Customer Care");
	this->customerSupportCallButton->setGeometry (670, 295, 59, 42);

	this->postpaidLinkButton = new ImageButton ("",
												":/postpaidDefault.png",
												":/postpaidHover.png",
												true, this);
	this->postpaidLinkButton->setToolTip ("Pay Postpaid bill online");
	this->postpaidLinkButton->setGeometry (555, 360, 186, 17);

	this->prepaidLinkButton = new ImageButton ("",
											   ":/prepaidDefault.png",
											   ":/prepaidHover.png",
											   true, this);
	this->prepaidLinkButton->setToolTip ("Pay Prepaid bill online");
	this->prepaidLinkButton->setGeometry (555, 375, 186, 17);

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(homeNetworkUpdate (QString)),
					  this,
					  SLOT(onHomeNetworkUpdate (QString)));

	QObject::connect (this->connectButton,
					  SIGNAL(clicked ()),
					  this,
					  SLOT(onConnectButtonClicked ()));

	QObject::connect (this->disconnectButton,
					  SIGNAL(clicked ()),
					  this,
					  SLOT(onDisconnectButtonClicked ()));

	QObject::connect (this->customerCareButton,
					  SIGNAL(clicked ()),
					  this,
					  SLOT(onCustomerSupportCallButtonClicked ()));

	QObject::connect (this->customerSupportLinkButton,
					  SIGNAL(clicked ()),
					  this,
					  SLOT(onCustomerSupportLinkButtonClicked ()));

	QObject::connect (this->customerSupportCallButton,
					  SIGNAL(clicked ()),
					  this,
					  SLOT(onCustomerSupportCallButtonClicked ()));

	QObject::connect (this->postpaidLinkButton,
					  SIGNAL(clicked ()),
					  this,
					  SLOT(onPostpaidLinkButtonClicked ()));

	QObject::connect (this->prepaidLinkButton,
					  SIGNAL(clicked ()),
					  this,
					  SLOT(onPrepaidLinkButtonClicked ()));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(modemPlugged (bool)),
					  this,
					  SLOT(onModemPlugged (bool)));

	QObject::connect (DeamonConnection::getInstance (),
					  SIGNAL(modemConnected (bool)),
					  this,
					  SLOT(onModemConnected (bool)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(pinCodeStatusUpdate (const QString&)),
					  this,
					  SLOT(onPinCodeUpdate (const QString&)));
}

bool
HomeWidget::onSignalStrengthUpdate (
		int value) {

	this->signalStrengthLabel->setPixmap (signalStrengthImage.at (value));
	return (true);
}

bool
HomeWidget::onConnectButtonClicked () {

	this->connectButton->hide ();
	this->disconnectButton->setEnabled (true);
	this->connectingButton->show ();

	DeamonConnection::getInstance ()->connectDataCall ();

	return (true);
}

bool
HomeWidget::onDisconnectButtonClicked () {

	this->disconnectButton->hide ();
	this->disconnectButton->setEnabled (false);
	this->disconnectingButton->show ();

	DeamonConnection::getInstance ()->disconnectDataCall ();

	return (true);
}

bool
HomeWidget::onModemPlugged (bool status) {

	if (true == status) {

		this->connectButton->setEnabled (true);
		this->disconnectButton->setEnabled (true);

	} else {

		this->modemPlugged = false;
        QMessageBox::information (NULL,
        							"Aircel",
                                    "The Device has been Disconnected or Unavailable",
                                    QMessageBox::Ok, QMessageBox::Ok);
		this->connectButton->setEnabled (false);
		this->disconnectButton->setEnabled (false);

		this->signalStrengthLabel->setPixmap (signalStrengthImage.at (0));
		this->homeNetworkLabel->setPixmap (this->homeNetworkList.at  (0));
		this->roamingNetworkLabel->setPixmap (this->roamingNetworkList.at (0));
	}
	return true;
}

bool
HomeWidget::onModemConnected (
		bool status) {

	if (true == status) {

		this->connectingButton->hide ();
		this->disconnectButton->show ();

	} else {

		this->connectingButton->hide ();
		this->disconnectingButton->hide ();
		this->disconnectButton->hide ();

		this->connectButton->show ();

	}
	return true;
}

bool HomeWidget::onHomeNetworkUpdate (QString value) {

	switch (value.toInt ()) {
		case 0: // fall through
		case 2: // fall through
		case 3: // fall through
		case 4: { // no network

			this->homeNetworkLabel->setPixmap (
						this->homeNetworkList.at (0));
			this->roamingNetworkLabel->setPixmap (
						this->roamingNetworkList.at (0));

		} break;

		case 1: { // local network
			this->homeNetworkLabel->setPixmap (
						this->homeNetworkList.at (1));
			this->roamingNetworkLabel->setPixmap (
						this->roamingNetworkList.at (0));
		} break;

		case 5: { // roaming network
			this->homeNetworkLabel->setPixmap (this->homeNetworkList.at (0));
			this->roamingNetworkLabel->setPixmap (this->roamingNetworkList.at (1));
		} break;
	}

	return (true);
}

bool
HomeWidget::onCustomerSupportCallButtonClicked () {

	emit dial  (CUSTOMER_CALL_NUMBER, false);

	return (true);
}

bool
HomeWidget::onCustomerSupportLinkButtonClicked () {

	QDesktopServices::openUrl (QUrl (WEBSITE));
	return (true);
}

bool
HomeWidget::onPrepaidLinkButtonClicked () {

	QDesktopServices::openUrl (QUrl (PREPAID_BILL_URL));
	return (true);
}

bool
HomeWidget::onPostpaidLinkButtonClicked () {

	QDesktopServices::openUrl (QUrl (POSTPAID_BILL_URL));
	return (true);
}

void
HomeWidget::paintEvent (
		QPaintEvent* event) {

	Q_UNUSED (event)

	QPainter painter (this);
	painter.drawImage (QPoint(0,0), *this->background);
	painter.end();
}

bool
HomeWidget::onPinCodeUpdate (
		QString value) {

	if(true == value.contains ("SIM PIN")) {

		PinActivateDialog pinActivate;
		pinActivate.exec ();

	} else if (true == value.contains ("SIM PUK")) {

		PinUnlockDialog pinUnlock;
		pinUnlock.exec ();

	}

	return true;
}

bool
HomeWidget::onResetNetwork () {

        this->homeNetworkLabel->setPixmap (this->homeNetworkList.at (0));
        this->roamingNetworkLabel->setPixmap (this->roamingNetworkList.at (0));

        return true;
}
