#ifndef OPERATOR_WIDGET
#define OPERATOR_WIDGET

#include "DeviceWidget.h"
#include "NetworkSettingWidget.h"
#include "NetworkStatusWidget.h"
#include "SimUsimWidget.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

class OperatorWidget : public QWidget {
	Q_OBJECT
public:
	explicit OperatorWidget(QWidget *parent = 0);

signals:

public slots:

	bool onSaveClicked();
	bool onResetClicked();

	bool onModemPlugged (bool status);
	bool onModemConnected (bool status);

protected:

private:
	DeviceWidget * deviceWidget;
	SimUsimWidget * simUsimWidget;
	NetworkStatusWidget * networkStatusWidget;
	NetworkSettingWidget * networkSettingWidget;

	QDialogButtonBox * buttons;

	QTabWidget * tabs;

	QVBoxLayout * layout;
};

#endif // OPERATOR_WIDGET
