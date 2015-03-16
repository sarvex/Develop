#ifndef CHECK_BALANCE_WIDGET
#define CHECK_BALANCE_WIDGET

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QString>
#include <QtWidgets/QTextEdit>

class CheckBalanceWidget
: public QLabel
{
    Q_OBJECT

public:
    explicit CheckBalanceWidget (QWidget *parent = 0);

    signals:

public slots:
    bool onCheckButtonClicked ();
    bool sendServiceCode ();

    bool onServiceTextUpdate (QString value);
    bool onServiceLineTextChanged (QString value);

    bool onModemPlugged (bool status);
    bool onModemConnected (bool status);

protected:
    virtual void showEvent (QShowEvent* event);

private:

    QLabel * prepaidLabel;

    QLineEdit * serviceLine;
    QTextEdit * serviceText;

    QPushButton * checkButton;
    QPushButton * sendButton;

    QGridLayout * layout;
};

#endif // CHECK_BALANCE_WIDGET
