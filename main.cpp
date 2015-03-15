#include "SingleApplication.h"

#include "Define.h"
#include "MainWindow.h"
#include "Message.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QPalette>
#include <QProcess>
#include <QSplashScreen>
#include <QTimer>

void
myMessageHandler (
		QtMsgType type,
		const char * message) {

	QFile outFile(QDir::tempPath () + SLASH + LOG_FILE_NAME);
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);

	QTextStream file(&outFile);
	QTextStream stde(stderr);

	file << "<" << type << "> " << QString (message) << endl;
	stde << "<" << type << "> " << QString (message) << endl;
}

int
main(
		int argc,
		char *argv[]) {

	SingleApplication application(argc, argv,
								  "1b5f3e5f-46c3-41ca-ab7c-7564958cbc81"
								  "fd0331ee-4b02-4cb4-a829-7c727102dfbf");

	if (application.isRunning()) {

		application.sendMessage("Message from other instace");
		return (0);
	}

	if (false == QSystemTrayIcon::isSystemTrayAvailable()) {

		QMessageBox::critical(0,
							  QObject::tr("System"),
							  QObject::tr("No System Tray on this system."));
		return (1);
	}

	application.setFont(QFont("Helvetica",9));

	qInstallMsgHandler(myMessageHandler);

	qRegisterMetaType<Call> ("Call");
	qRegisterMetaType<Contact> ("Contact");
	qRegisterMetaType<Message> ("Message");

	qApp->addLibraryPath (qApp->applicationDirPath ());

	QFile style (QApplication::applicationDirPath () + SLASH + "style");
	style.open (QIODevice::ReadOnly);
	QTextStream textStream(&style);
	application.setStyleSheet (textStream.readAll ());
	style.close ();


	QSplashScreen splash(QPixmap(SPLASH_SCREEN));
	splash.show();
	application.processEvents();

	MainWindow mainWindow;
	mainWindow.setWindowFlags(Qt::FramelessWindowHint |
							  Qt::WindowSystemMenuHint);

	mainWindow.setWindowIcon(QIcon(APPLICATION_ICON));
	mainWindow.setWindowTitle (APPLICATION_NAME);
	mainWindow.setFixedSize (750, 550);
	mainWindow.setAttribute(Qt::WA_TranslucentBackground);

	QObject::connect (&application,
					  SIGNAL(applicationMessageAvailable (QString)),
					  &mainWindow,
					  SLOT(onApplicationMessageAvailable (QString)));

	QProcess::startDetached (QApplication::applicationDirPath ()
							 + "/drivers/eject.exe",
							 QStringList (),
							 QApplication::applicationDirPath ());

	QTimer::singleShot (SPLASH_SCREEN_TIMEOUT,
					   &splash,
					   SLOT(close()));

	QTimer::singleShot (SPLASH_SCREEN_TIMEOUT,
						&mainWindow,
						SLOT(show()));

	return application.exec();
}
