#ifndef VOICE_WIDGET
#define VOICE_WIDGET

#include "Call.h"
#include "ImageButton.h"

#include <QtGui/QIcon>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtWidgets/QSlider>
#include <QtCore/QString>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtWidgets/QWidget>

class VoiceWidget
		: public QWidget {
	Q_OBJECT

public:
	explicit VoiceWidget (QWidget *parent = 0);

	bool dial (QString number, bool connect);

signals:
	bool readContactUpdate (bool enable);
	bool dialed (Call calling);
	bool answered (Call calling);
	bool missed (Call calling);
	bool number (QString number);

public slots:
	bool name (QString number, QString name);

	bool onIncomingCallUpdate (Call calling);
	bool onCallAcceptUpdate ();
	bool onCallEndUpdate ();

	bool onTableItemDoubleClicked (QTableWidgetItem*);
	bool onContextMenu (const QPoint&);

	bool onVolumeSliderValueChanged (int value);
	bool onDialingNumberTextChanged ();

	bool onMuteMicrophoneClicked ();
	bool onMuteSpeakerClicked ();

	bool onVolumeUpClicked();
	bool onVolumeDownClicked();

	bool onCallButtonClicked ();
	bool onHangButtonClicked ();
	bool onDialOneButtonClicked ();
	bool onDialTwoButtonClicked ();
	bool onDialThreeButtonClicked ();
	bool onDialFourButtonClicked ();
	bool onDialFiveButtonClicked ();
	bool onDialSixButtonClicked ();
	bool onDialSevenButtonClicked ();
	bool onDialEightButtonClicked ();
	bool onDialNineButtonClicked ();
	bool onDialZeroButtonClicked ();
	bool onDialStarButtonClicked ();
	bool onDialHashButtonClicked ();
	bool onPhoneBookClicked ();
	bool onDialPlusButtonClicked ();
	bool onDialCancelButtonClicked ();
	bool onDialBackButtonClicked ();

	bool onCallTimer ();

	bool onModemPlugged (bool status);
	bool onModemConnected (bool status);

protected:
	virtual void showEvent (QShowEvent * event);

private:
	bool initialize ();
	bool createDialpad ();
	bool createTable ();
	bool createLayout ();

	bool connect ();

	bool callToRow (Call calling);
	Call rowToCall (int index);

	bool dial ();

	QLabel * voiceCallBackground;
	QLabel * background;

	ImageButton * microphoneButton;
	ImageButton * speakerButton;
	ImageButton * volumeUpButton;
	ImageButton * volumeDownButton;

	QLabel * recommendationLabel;

	QLineEdit * dialingNumber;
	QTableWidget * table;

	QTime startTime;
	QTime duration;
	QTimer * callTimer;

	QSlider * volumeSlider;

	ImageButton * callButton;
	ImageButton * hangButton;
	ImageButton * dialOne;
	ImageButton * dialTwo;
	ImageButton * dialThree;
	ImageButton * dialFour;
	ImageButton * dialFive;
	ImageButton * dialSix;
	ImageButton * dialSeven;
	ImageButton * dialEight;
	ImageButton * dialNine;
	ImageButton * dialZero;
	ImageButton * dialStar;
	ImageButton * dialHash;
	ImageButton * phonebook;
	ImageButton * dialPlus;
	ImageButton * dialClear;
	ImageButton * dialBack;

	QList <QIcon> icon;

	bool incomingCall;
	bool calling;
};

#endif // VOICE_WIDGET
