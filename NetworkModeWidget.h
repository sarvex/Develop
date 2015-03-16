#ifndef NETWORK_MODE_WIDGET
#define NETWORK_MODE_WIDGET

#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

class NetworkModeWidget : public QWidget {
	Q_OBJECT
public:
	explicit NetworkModeWidget(QWidget *parent = 0);

signals:
	bool resetNetwork ();

public slots:
	bool onApplyClicked ();
	bool onResetClicked ();

	bool onNetworkModeChange(int index);
	bool onGetSelectedNetworkModeUpdate (QString value);

	bool onModemConnected (bool status);
	bool onModemPlugged (bool status);

protected:
	virtual void showEvent (QShowEvent* event);

private:
	QComboBox * networkMode;

	QDialogButtonBox * buttons;

	QFormLayout * formLayout;
	QVBoxLayout * layout;

};

#endif // NETWORK_MODE_WIDGET
