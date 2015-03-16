#ifndef NETWORK_SETTING_WIDGET
#define NETWORK_SETTING_WIDGET

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtCore/QString>
#include <QtWidgets/QWidget>

class NetworkSettingWidget
		: public QWidget {
	Q_OBJECT

public:
	explicit NetworkSettingWidget(QWidget *parent = 0);

	QString readAll ();
	bool clear ();

signals:

public slots:
	bool onAccessPointNameUpdate (QString value);
	bool onNetworkSelectionModeUpdate (QString value);

	bool onModemConnected (bool status);
	bool onModemPlugged (bool status);

    bool onTimer();

protected:
	virtual void showEvent (QShowEvent* event);

private:
	QLineEdit * apn;
	QLineEdit * networkSelectionMode;

	QFormLayout * layout;
	QStringList mode;
};

#endif // NETWORK_SETTING_WIDGET
