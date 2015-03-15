#ifndef MESSAGE_SETTING_WIDGET
#define MESSAGE_SETTING_WIDGET

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWidget>

class MessageSettingWidget
		: public QLabel {
	Q_OBJECT

public:
	explicit MessageSettingWidget(QWidget *parent = 0);

signals:

public slots:
	bool onApplyClicked ();
	bool onDeliveryReport (int state);
	bool onChanged ();

	bool onModemPlugged (bool status);
	bool onModemConnected (bool status);

protected:

private:
	QGroupBox * groupBox;

	QRadioButton * saveLocalRadioButton;
	QRadioButton * saveSimRadioButton;

	QCheckBox * delieveryReport;

	QDialogButtonBox * buttons;

	QVBoxLayout * groupLayout;
	QVBoxLayout * layout;
};

#endif // MESSAGE_SETTING_WIDGET
