#ifndef PIN_DIALOG
#define PIN_DIALOG

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QVBoxLayout>

class PinDialog
	: public QDialog {
	Q_OBJECT

public:
	explicit PinDialog(QWidget *parent = 0);
	QString getPin ();

signals:

public slots:
	bool onOkClicked ();
	bool onCancelClicked ();
	bool onAttemptUpdate (QString value);
	bool onPinTextChanged (QString text);

protected:
	virtual void showEvent (QShowEvent *event);

private:
	QLineEdit * pin;
	QLineEdit * attempt;

	QDialogButtonBox * buttons;

	QVBoxLayout * layout;
	QFormLayout * formLayout;
};

#endif // PIN_DIALOG
