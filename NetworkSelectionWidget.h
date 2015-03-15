#ifndef NETWORK_SELECTION_WIDGET
#define NETWORK_SELECTION_WIDGET

#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QRadioButton>
#include <QSplitter>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>

class NetworkSelectionWidget
	: public QWidget {
	Q_OBJECT

public:
	explicit NetworkSelectionWidget(QWidget *parent = 0);

signals:

public slots:
	bool onNetworkModeUpdate (QString mode,
							  QString major,
							  QString minor,
							  QString identifier,
							  QString status);

	bool onTableItemClicked (QTableWidgetItem* item);

	bool onRefreshButtonClicked ();
	bool onRegisterButtonClicked ();
	bool onAutomaticButtonClicked ();
	bool onManualButtonClicked ();

	bool onModemConnected (bool status);
	bool onModemPlugged (bool status);

protected:
	virtual void showEvent (QShowEvent * event);

private:
	bool updateNetworkMode ();

	QProgressBar * progressBar;

	QPushButton * refreshButton;
	QPushButton * registerButton;

	QRadioButton * automaticButton;
	QRadioButton * manualButton;

	QTableWidget * table;

	QTextEdit * content;

	QSplitter * splitter;
	QSplitter * reader;

	QGridLayout * layout;

	QStringList mode;
	QStringList status;
	QStringList header;
};

#endif // NETWORK_SELECTION_WIDGET
