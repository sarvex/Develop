#ifndef MAIN_WINDOW
#define MAIN_WINDOW

#include "AnimateButton.h"
#include "ContactWidget.h"
#include "Define.h"
#include "HomeWidget.h"
#include "ImageButton.h"
#include "MessageWidget.h"
#include "SpeedLabel.h"
#include "SettingWidget.h"
#include "TimeLabel.h"
#include "VoiceWidget.h"

#include <QtCore/QByteArray>
#include <QtCore/QBuffer>
#include <QtNetwork>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>
#include <QSystemTrayIcon>
#include <QtCore/QTranslator>
#include <QtCore/QUrl>
#include <QtWidgets/QVBoxLayout>

class MainWindow
		: public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	void setVisible(bool visible);

public slots:
	bool onResetNetwork ();

	bool onMessageAnnunciator (bool visible);
	bool onCallAnnunciator (bool visible);

	bool onCallAnnunciatorClicked();
	bool onMessageAnnunciatorClicked ();

	bool onApplicationMessageAvailable (QString message);
	bool onSystemTrayActivated (QSystemTrayIcon::ActivationReason reason);

	bool onVersionRequestFinished (int index, bool failed);
	bool onSetupRequestFinished (int index, bool failed);

	bool onDial (QString number, bool connect);
	bool onSave (QString number);
	bool onSend (QString number, QString content);


	bool onMinimizeButtonClicked();
	bool onCloseButtonClicked();

	bool onHomeButtonClicked();
	bool onMessageButtonClicked();
	bool onPhonebookButtonClicked();
	bool onGamesButtonClicked();
	bool onMusicButtonClicked();
	bool onSettingButtonClicked();
	bool onVoiceButtonClicked();

	bool onAnswerTriggered();
	bool onHangupTriggered();
	bool onSilentTriggered();

	bool onMessageRecievedUpdate (Message message);
	bool onMessageReceivedSimUpdate (QString index);
	bool onIncomingCallUpdate (Call call);
	bool onDeliveryReportUpdate (Message message);
	bool onDeliveryReportSimUpdate (QString index);
	bool onOpenWebPage(QString url);

	bool onModemPlugged (bool status);
	bool onModemConnected (bool status);
	bool onRasError (int errorNumber,
						QString errorString);

	bool onNetworkDisplayModeUpdate(QString value,
									QString networkName);

	bool onSignalStrengthUpdate (QString value);

	bool onUpgradeTriggered ();
	bool onQuitTriggered ();

signals:
	bool selectUnread ();

protected:
	virtual void closeEvent (QCloseEvent* event);

	virtual void paintEvent(QPaintEvent* event);

	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);

private:

	bool createSystemTray();
	bool createBackground ();
	bool createDisplay ();
	bool createStatusBar ();

	bool initialize ();
	bool connect ();

	bool close ();

	bool checkVersion ();

	bool resetTabs ();

	QStackedWidget * display;

	HomeWidget * home;
	MessageWidget * message;
	ContactWidget * phonebook;
	SettingWidget * setting;
	VoiceWidget * voice;

	Tabs activeTab;

	ImageButton * minimizeButton;
	ImageButton * closeButton;
	ImageButton * homeButton;
	ImageButton * messageButton;
	ImageButton * phonebookButton;
	ImageButton * gamesButton;
	ImageButton * musicButton;
	ImageButton * settingButton;
	ImageButton * voiceButton;

	AnimateButton * messageAnnunciator;
	AnimateButton * callAnnunciator;
	AnimateButton * sessionLabel;

	SpeedLabel* speedLabel;
	TimeLabel * timeLabel;

	QLabel * header;
	QLabel * footer;
	QLabel * networkDisplayMode;
	QLabel * dataRate;
	QLabel * signalStrength;
	QLabel * signalStrengthLabel;
	QLabel * dataConnectionLabel;

	QSystemTrayIcon *trayIcon;
	QMenu * trayIconMenu;
	QMenu * callMenu;

	QAction * callAction;
	QAction * answerAction;
	QAction * hangupAction;
	QAction * silentAction;
	QAction * minimizeAction;
	QAction * restoreAction;
	QAction * upgradeAction;
	QAction * quitAction;

	QStringList signalStrengths;

	QList<QPixmap> signalStrengthImage;
	QList<QPixmap> dataConnectionImage;

	QImage * background;

	QStringList mode;
	QPoint mouseOffset;

	QHttp * versionRequest;
	QHttp * setupRequest;

	QBuffer * versionBuffer;
	QBuffer * setupBuffer;

	QByteArray * versionData;
	QByteArray * setupData;

	int versionRequestIndex;
	int setupRequestIndex;

	bool moving;
	bool plugged;
	bool connected;
};

#endif // MAIN_WINDOW
