#ifndef SIM_USIM_WIDGET
#define SIM_USIM_WIDGET

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtCore/QString>
#include <QtWidgets/QWidget>

class SimUsimWidget
		: public QWidget {
	Q_OBJECT

public:
	explicit SimUsimWidget(QWidget *parent = 0);

	QString readAll ();
	bool clear ();

signals:

public slots:
	bool onOwnNumberUpdate (QString value);

	bool onMessageCountUpdate (QString total, QString capacity);

	bool onContactCountUpdate (QString total, QString capacity);

	bool onMessageCenterNumberUpdate (QString value);

	bool onPinCodeUpdate (QString value);

	bool onModemPlugged (bool status);
	bool onModemConnected (bool status);

	bool onTimer();
protected:
	virtual void showEvent (QShowEvent * event);

private:
	QLineEdit * ownNumber;
	QLineEdit * messageSim;
	QLineEdit * contactSim;
	QLineEdit * messageCenterNumber;
	QLineEdit * pinCode;

	QFormLayout * layout;
};

#endif // SIM_USIM_WIDGET
