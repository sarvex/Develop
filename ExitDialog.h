#ifndef EXIT_DIALOG
#define EXIT_DIALOG

#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

class ExitDialog
	: public QDialog {
	Q_OBJECT

public:
	explicit ExitDialog(QDialog *parent = 0);

signals:

public slots:
	bool onShutDownDeviceClicked();
	bool onQuitApplicationClicked();
	bool onCancelClicked();

	bool onModemPlugged (bool status);
	bool onModemConnected (bool status);

protected:

private:
	QLabel * message;
	QDialogButtonBox * buttons;

	QVBoxLayout * layout;
};

#endif // EXIT_DIALOG
