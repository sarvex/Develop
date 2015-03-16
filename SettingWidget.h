#ifndef SETTING_WIDGET
#define SETTING_WIDGET

#include "Call.h"
#include "CallWidget.h"
#include "DataWidget.h"
#include "CheckBalanceWidget.h"
#include "MessageSettingWidget.h"
#include "NetworkWidget.h"
#include "OperatorWidget.h"
#include "PinWidget.h"
#include "PocketWidget.h"
#include "UserManualWidget.h"

#include <QtWidgets/QListWidget>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtCore/QString>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

class SettingWidget
		: public QWidget {
	Q_OBJECT

public:
	explicit SettingWidget (QWidget *parent = 0);

	bool updateTree ();

signals:
	bool dialed (Call call);
	bool answered (Call call);
	bool missed (Call call);
	bool selectUnread ();

	bool resetNetwork ();

	bool callAnnunciator (bool);
	bool dial (QString number, bool connect);
	bool save (QString number);
	bool send (QString number, QString content);
	bool number (QString number);
	bool name (QString number, QString name);

public slots:
	bool onSelectUnread ();

protected:

private:
	CallWidget * callWidget;
	DataWidget * dataWidget;
	CheckBalanceWidget * checkBalanceWidget;
	MessageSettingWidget * messageSettingWidget;
	NetworkWidget * networkWidget;
	OperatorWidget * operatorWidget;
	PinWidget * pinWidget;
	PocketWidget * pocketWidget;
	UserManualWidget * userManualWidget;

	QListWidget * list;
	QStackedWidget * content;

	QSplitter * splitter;
	QVBoxLayout * layout;
};

#endif // SETTING_WIDGET
