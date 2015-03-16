#ifndef POCKET_WIDGET
#define POCKET_WIDGET

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QTCore/QString>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

class PocketWidget
		: public QLabel {
	Q_OBJECT

public:
	explicit PocketWidget (QWidget *parent = 0);

signals:

public slots:
	bool onSoftwareVersionUpdate (QString value);

	bool onModemPlugged (bool status);
	bool onModemConnected (bool status);

protected:
	virtual void showEvent (QShowEvent* event);

private:
	QLabel * pocketInternet;
	QLabel * copyright;

	QLineEdit * softwareVersion;
	QTextEdit * warning;

	QGroupBox * groupBox;

	QGridLayout * layout;
};

#endif // POCKET_WIDGET
