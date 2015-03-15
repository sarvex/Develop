#ifndef SINGLE_APPLICATION
#define SINGLE_APPLICATION

#include <QApplication>
#include <QLocalServer>
#include <QSharedMemory>
#include <QString>

class SingleApplication
		: public QApplication {
	Q_OBJECT

public:
	SingleApplication (int &argc,
					   char *argv[],
					   QString uniqueKey);

	bool isRunning();
	bool sendMessage (QString message);

public slots:
	bool applicationMessage();

signals:
	bool applicationMessageAvailable (QString message);

protected:

private:
	bool running;
	QString uniqueKey;
	QSharedMemory sharedMemory;
	QLocalServer *localServer;

	static const int timeout = 1000;
};

#endif // SINGLE_APPLICATION
