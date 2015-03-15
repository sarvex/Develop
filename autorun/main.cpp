#include <QtGui/QApplication>
#include "MainWindow.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

void myMessageHandler(QtMsgType messageType, const char *message) {
    QString logMessage;

    switch (messageType) {
    case QtDebugMsg: {
            logMessage = QString("Debug: %1").arg(message);
            fprintf(stderr, "Debug: %s\n", message);
        }break;

    case QtWarningMsg: {
            logMessage = QString("Warning: %1").arg(message);
            fprintf(stderr, "Warning: %s\n", message);
        }break;

    case QtCriticalMsg: {
            logMessage = QString("Critical: %1").arg(message);
            fprintf(stderr, "Critical: %s\n", message);
        }break;

    case QtFatalMsg: {
            logMessage = QString("Fatal: %1").arg(message);
            fprintf(stderr, "Fatal: %s\n", message);
            abort();
        }break;
    }

    QFile outFile(QDir::tempPath () + "/autorun");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream textStream(&outFile);
    textStream << logMessage << endl;
}


int
main(
        int argc, char *argv[]) {

    qInstallMsgHandler(myMessageHandler);

    QApplication application(argc, argv);
    MainWindow mainWindow;

    Q_UNUSED (application);
    Q_UNUSED (mainWindow);

    return 0;
}
