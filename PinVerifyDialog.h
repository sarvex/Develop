#ifndef PIN_VERIFY_DIALOG
#define PIN_VERIFY_DIALOG

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>

class PinVerifyDialog : public QDialog
{
	Q_OBJECT
public:
	explicit PinVerifyDialog(QWidget *parent = 0);
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

#endif // PIN_VERIFY_DIALOG
