#ifndef DATA_WIDGET
#define DATA_WIDGET

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class DataWidget
		: public QLabel {
	Q_OBJECT

public:
	explicit DataWidget (QWidget *parent = 0);

signals:
	bool resetUsage ();

public slots:
	bool onResetClicked ();

	bool onUsageUpdate (double upload, double download);

	bool onModemPlugged (bool status);
	bool onModemConnected (bool status);

protected:
	virtual void showEvent (QShowEvent * event);

private:
	QLineEdit * uploadData;
	QLineEdit * downloadData;
	QLabel * uploadUsage;
	QLabel * downloadUsage;
	QLabel * note;

	QDialogButtonBox * buttons;

	QGroupBox * groupBox;

	QFormLayout * formLayout;
	QVBoxLayout * layout;

	double upload;
	double download;
};

#endif // DATA_WIDGET
