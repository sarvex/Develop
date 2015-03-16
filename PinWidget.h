#ifndef PIN_WIDGET
#define PIN_WIDGET

#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

class PinWidget
		: public QLabel {
	Q_OBJECT

public:
	explicit PinWidget(QWidget *parent = 0);

signals:

public slots:
	bool onEnableButtonClicked ();
	bool onModifyButtonClicked ();
	bool onVerifyButtonClicked ();
	bool onUnlockButtonClicked ();

	bool onPinQueryUpdate (QString value);
	bool onPinCodeUpdate (QString value);
	bool onIncorrectPinPukUpdate (QString value);
	bool onPinModifyUpdate (QString value);

	bool onModemPlugged (bool status);
	bool onModemConnected (bool status);

protected:
	virtual void showEvent (QShowEvent* event);

private:
	QGroupBox * enableGroupBox;
	QGroupBox * verifyGroupBox;
	QGroupBox * unlockGroupBox;

	QLabel * enableLabel;
	QLabel * verifyLabel;
	QLabel * unlockLabel;

	QVBoxLayout * enableLayout;
	QVBoxLayout * verifyLayout;
	QVBoxLayout * unlockLayout;

	QVBoxLayout * layout;

	QDialogButtonBox * enableGroupButtons;
	QDialogButtonBox * verifyGroupButtons;
	QDialogButtonBox * unlockGroupButtons;

	QPushButton * enableButton;
	QPushButton * modifyButton;
	QPushButton * verifyButton;
	QPushButton * unlockButton;
};

#endif // PIN_WIDGET
