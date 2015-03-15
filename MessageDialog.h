#ifndef MESSAGE_DIALOG
#define MESSAGE_DIALOG

#include "Message.h"

#include <QDateTime>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QTextEdit>

class MessageDialog : public QDialog {
	Q_OBJECT
public:
	enum Result {
		Send,
		Save,
		Cancel,
		Count
	};

	explicit MessageDialog(QWidget *parent = 0);
	MessageDialog (const QString& number,
				   const QString& content,
				   QWidget *parent = 0);

	QList<Message> getMessages ();

signals:

public slots:

	bool onNumberTextChanged (QString number);
	bool onContentTextChanged ();

	bool onSendToClicked ();
	bool onSendClicked ();
	bool onSaveClicked ();
	bool onCancelClicked ();

	bool onModemPlugged (bool status);

protected:

private:
	bool initialize ();
	bool createMessages (Message::Types type);

	QLineEdit * number;
	QTextEdit * content;

	QPushButton * sendTo;

	QDialogButtonBox * buttons;

	QLabel * messageCount;
	QLabel * characterCount;

	QGridLayout * layout;

	QList<Message> messages;
	QString data;
};

#endif // MESSAGE_DIALOG
