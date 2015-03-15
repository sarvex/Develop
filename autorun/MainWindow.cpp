#include "MainWindow.h"
#include "SerialPort.h"
#include "SerialEnumerator.h"

#include <QApplication>
#include <QDebug>
#include <QList>
#include <QProcess>
#include <QSettings>

#include <windows.h>
#include <ShellAPI.h>

MainWindow::MainWindow(
		QWidget *parent)
	: QMainWindow(parent) {


	qDebug () << "Checking Registry";
	if (false == this->checkRegistry ()) {

		qDebug () << "Application not installed, run setup";
		this->runSetup ();

	} else {

		qDebug () << "Application found running the application";
		this->runApplication ();

		qDebug () << "Checking the modem";
		if (false == this->checkModem ()) {

			qDebug () << "Ejecting the modem";
			this->eject ();
		}
	}
}

MainWindow::~MainWindow() {
}

bool MainWindow::checkRegistry () {
	qDebug () << "Entering MainWindow::checkRegistry";

	QSettings setting("HKEY_LOCAL_MACHINE\\Software\\Wow6432Node\\Aircel", QSettings::NativeFormat);
	this->installDirectory = setting.value ("InstallationDirectory").toString ();
	this->amd = true;

	if (true == this->installDirectory.isEmpty ()) {

		QSettings setting("HKEY_LOCAL_MACHINE\\Software\\Aircel", QSettings::NativeFormat);
		this->installDirectory = setting.value ("InstallationDirectory").toString ();
		this->amd = false;
	}

	this->installDirectory.replace ("\\", "/");

	qDebug () << "Exiting MainWindow::checkRegistry";
	return (false == this->installDirectory.isEmpty ());
}

bool MainWindow::runSetup () {
	qDebug () << "Entering MainWindow::runSetup";

	QString exeFileName ("setup.exe");

	int result = (int)::ShellExecuteA(0, "open", exeFileName.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
	
	if (SE_ERR_ACCESSDENIED == result) {
		// Requesting elevation
		result = (int)::ShellExecuteA(0, "runas", exeFileName.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
	} 

	if (result <= 32) {
		// error handling
	}

	QProcess process;
	process.start (exeFileName);
	process.waitForFinished (30*60*1000);

	qDebug () << "Exiting MainWindow::runSetup";

	this->checkRegistry();
	return this->eject();
}

bool MainWindow::checkModem () {
	qDebug () << "Entering MainWindow::checkModem";

	QList<PortInformation> ports = SerialEnumerator::getPorts();

	foreach (PortInformation port, ports) {
		if (0x22F4 == port.vendorID) {
			if (0x0026 == port.productID) {
				if (port.friendName.contains("Olive Interface")) {
					return true;
				}
			}
		}
	}

	qDebug () << "Exiting MainWindow::checkModem";
	return false;
}

bool MainWindow::eject () {
	qDebug () << "Entering MainWindow::eject";

	qDebug () << "Eject path" << this->installDirectory + "/drivers/eject.exe";
	qDebug () << "Parameters" << qApp->applicationDirPath ().split (":").at (0);
	qDebug () << "Installation Directory" << this->installDirectory;

	QProcess::startDetached (this->installDirectory + "/drivers/eject.exe",
							 QStringList () << qApp->applicationDirPath ().split (":").at (0),
							 this->installDirectory);

	qDebug () << "Exiting MainWindow::eject";
	return true;
}

bool MainWindow::runApplication () {
	qDebug () << "Exiting MainWindow::runApplication";

	qDebug () << "Running" << this->installDirectory + "/Aircel.exe";
	QProcess::startDetached (this->installDirectory + "/Aircel.exe", QStringList (), this->installDirectory);

	qDebug () << "Exiting MainWindow::runApplication";
	return true;
}
