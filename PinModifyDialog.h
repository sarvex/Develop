#ifndef PIN_MODIFY_DIALOG
#define PIN_MODIFY_DIALOG

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QVBoxLayout>

class PinModifyDialog : public QDialog {
	Q_OBJECT

public:
	explicit PinModifyDialog(QWidget *parent = 0);

	QString getNewPin ();

signals:

public slots:
	bool onCurrentPinTextChanged(QString text);
	bool onNewPinTextChanged(QString text);
	bool onConfirmPinTextChanged(QString text);

	bool onAttemptUpdate (const QString& value);

	bool onOkClicked ();
	bool onCancelClicked ();

protected:
	virtual void showEvent (QShowEvent *event);

private:
	QLineEdit * currentPin;
	QLineEdit * newPin;
	QLineEdit * confirmPin;
	QLineEdit * attempt;

	QRegExpValidator * validator;

	QDialogButtonBox * buttons;

	QFormLayout * formLayout;
	QVBoxLayout * layout;
};

#endif // PIN_MODIFY_DIALOG
