#ifndef PIN_UNLOCK_DIALOG
#define PIN_UNLOCK_DIALOG

#include <QDialog>
#include <QDialogButtonBox>

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

class PinUnlockDialog : public QDialog
{
	Q_OBJECT
public:
	explicit PinUnlockDialog(QWidget *parent = 0);

	QString getNewPin ();

signals:

public slots:
	bool onConfirmPinTextChanged(QString text);
	bool onNewPinTextChanged(QString text);
	bool onCurrentPukTextChanged(QString text);

	bool onAttemptUpdate (QString value);

	bool onOkClicked ();
	bool onCancelClicked ();

protected:
	virtual void showEvent (QShowEvent *event);

private:
	QLineEdit * currentPuk;
	QLineEdit * newPin;
	QLineEdit * confirmPin;
	QLineEdit * attempt;

	QDialogButtonBox * buttons;

	QFormLayout * formLayout;
	QVBoxLayout * layout;
};

#endif // PIN_UNLOCK_DIALOG
