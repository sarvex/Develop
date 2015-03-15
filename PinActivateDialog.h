#ifndef PIN_ACTIVATE_DIALOG
#define PIN_ACTIVATE_DIALOG

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QVBoxLayout>

class PinActivateDialog
		: public QDialog {
	Q_OBJECT

public:
	explicit PinActivateDialog(QWidget *parent = 0);
	QString getPin ();

signals:

public slots:
	bool onPinTextChanged(QString text);
	bool onAttemptUpdate (QString value);

	bool onOkClicked ();
	bool onCancelClicked ();

protected:
	virtual void showEvent (QShowEvent *event);

private:
	QLineEdit * pin;
	QLineEdit * attempt;

	QDialogButtonBox * buttons;

	QVBoxLayout * layout;
	QFormLayout * formLayout;
};

#endif // PIN_ACTIVATE_DIALOG
