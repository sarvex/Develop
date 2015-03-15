#ifndef DEAMON_WIRELESS
#define DEAMON_WIRELESS

#include <QThread>

class DeamonWireless : public QThread
{
    Q_OBJECT
public:
    explicit DeamonWireless(QObject *parent = 0);

signals:

public slots:

protected:

private:

};

#endif // DEAMON_WIRELESS
