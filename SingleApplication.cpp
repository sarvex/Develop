#include <QLocalSocket>

#include "SingleApplication.h"

SingleApplication::SingleApplication (
		int &argc,
		char *argv[],
		QString uniqueKey)
	: QApplication(argc, argv),
	  uniqueKey(uniqueKey) {

	this->sharedMemory.setKey(uniqueKey);

	if (this->sharedMemory.attach()) {

		this->running = true;

	} else {

		this->running = false;

		// create shared memory.
		if (false == this->sharedMemory.create(1)) {
			qDebug("Unable to create single instance.");
			return;
		}

		// create local server and listen to incoming messages from other instances.
		this->localServer = new QLocalServer(this);
		QObject::connect (this->localServer, SIGNAL(newConnection()), SLOT(applicationMessage()));
		localServer->listen(uniqueKey);

	}
}

bool
SingleApplication::applicationMessage() {

	QLocalSocket *localSocket = localServer->nextPendingConnection();

	if (false == localSocket->waitForReadyRead(timeout)) {

		qDebug(localSocket->errorString().toLatin1());
		return false;
	}

	QByteArray byteArray = localSocket->readAll();

	QString message = QString::fromUtf8(byteArray.constData());

	emit applicationMessageAvailable(message);
	localSocket->disconnectFromServer();

	return true;
}

bool
SingleApplication::isRunning() {

	return (running);
}

bool
SingleApplication::sendMessage (
		QString message) {

	if (false == running) {
		return (false);
	}

	QLocalSocket localSocket(this);
	localSocket.connectToServer(uniqueKey, QIODevice::WriteOnly);

	if (false == localSocket.waitForConnected(timeout)) {

		qDebug(localSocket.errorString().toLatin1());
		return (false);
	}

	localSocket.write(message.toUtf8());

	if (false == localSocket.waitForBytesWritten(timeout)) {

		qDebug(localSocket.errorString().toLatin1());
		return (false);
	}

	localSocket.disconnectFromServer();

	return (true);
}
